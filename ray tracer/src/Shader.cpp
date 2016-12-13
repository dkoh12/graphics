#include "Shape.h"
#include "Light.h"
#include <cmath>
#include <math.h>


#include "Matrices.cpp"
#include "Matrices.h"

#include <vector>
#include <string>


#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define PI 3.14159265

Ray calculateInvertRay(Matrix4 world2obj, Ray ray) {
    
    Vector4 xformed_ray_point(ray.orig[0], ray.orig[1], ray.orig[2], 1);
    Vector4 xformed_ray_dir(ray.dir[0], ray.dir[1], ray.dir[2], 0); 

    world2obj.invert();

    xformed_ray_point =  world2obj * xformed_ray_point;
    xformed_ray_dir =  world2obj * xformed_ray_dir;

    Vector3 ppoint = Vector3(xformed_ray_point[0], xformed_ray_point[1], xformed_ray_point[2]);
    Vector3 ddir = Vector3(xformed_ray_dir[0], xformed_ray_dir[1], xformed_ray_dir[2]);
    ddir.normalize();

    Ray inverted_ray(ppoint, ddir);

    return inverted_ray;
}

Matrix4 calculateMatrix(Sphere sph, vector<vector<Vector4>> &venus, int k) {

    Matrix4 M = Matrix4(); //identity

    for (int B = venus[k].size()-1 ; B >= 0; B--) {
        Vector4 temp = venus[k][B];
        Matrix4 mat;
        // translation
        if (temp[0] == 0) {
            mat = Matrix4(1, 0, 0, temp[1],
                        0, 1, 0, temp[2],
                        0, 0, 1, temp[3],
                        0, 0, 0, 1);
            M = mat.transpose() * M;

        //rotation
        } else if(temp[0] == 1) {
            float x = temp[1];
            float y = temp[2];
            float z = temp[3];

            float mag = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
            float theta = mag * PI / 180;
            if (mag > 0) {
                x /= mag;
                y /= mag;
                z /= mag;
            }
            float c = cosf(theta);
            float s = sinf(theta);
            float t = 1.0 - c;

            mat = Matrix4(t*x*x + c, t*x*y - z*s, t*x*z + y*s, 0,
                        t*x*y + z*s, t*y*y + c, t*y*z - x*s, 0,
                        t*x*z - y*s, t*y*z + x*s, t*z*z + c, 0,
                        0, 0, 0, 1);

            M = mat.transpose() * M;

        //scaling
        } else if(temp[0] == 2) {
            mat = Matrix4(temp[1], 0, 0, 0,
                        0, temp[2], 0, 0,
                        0, 0, temp[3], 0,
                        0, 0, 0, 1);

            M = mat * M;
        }
    }

    // translates sphere from origin to original place
    Matrix4 toPlace(1, 0, 0, sph.center.x,
                0, 1, 0, sph.center.y,
                0, 0, 1, sph.center.z,
                0, 0, 0, 1);

    M = toPlace.transpose() * M;

    return M;
}

Vector3 calculateShade (Material m, Vector3 L, Vector3 N, Vector3 V, Vector3 r, Vector3 I) {
    Vector3 ka = m.ka;
    Vector3 kd = m.kd;
    Vector4 ks = m.ks;
    
    // Vector3 ambient = ka * I;

    Vector3 diffuse = kd * I * MAX(L.dot(N), 0);

    Vector3 ks_v (ks.x, ks.y, ks.z);
    float sp = ks.w;
    float power = pow(MAX(r.dot(V), 0), sp);
    Vector3 specular = ks_v * I * power;

    // Vector3 R = ambient + diffuse + specular;
    Vector3 R = diffuse + specular;
    return R;
}

Vector3 returnIntersect (vector<Sphere> &sph, vector<Triangle> &tri, Ray &ray, 
    int which_object, int index, vector<vector<Vector4>> &venus, Ray &closest_ray) {
    
    float this_t = 1000;

    float hitType = -1;
    float hitIndex = -1;
    float hitDistance = -1;

    // for each sphere
    for(int k = 0; k<sph.size(); k++) {
        Sphere s = sph[k];
        Ray thisRay = ray;

        if (venus[k].size() > 0) {
            Matrix4 M = calculateMatrix(sph[k], venus, k);
            thisRay = calculateInvertRay(M, ray);
            s = Sphere(Vector3(0,0,0), sph[k].radius);
        }

        float f = s.intersect(thisRay);
        if (f != -1) {
            if (f < this_t) {
                if (which_object == 1 && index == k) {
                    continue;
                }
                hitType = 1; // sphere
                hitIndex = k;
                hitDistance = f;
                this_t = f;
                closest_ray = thisRay;
            }
        }
    }

    // for each triangle
    for(int k = 0; k< tri.size(); k++) {
        float f = tri[k].intersect(ray);
        if (f != -1) {
            if (f < this_t) {
                if (which_object == 2 && index == k) {
                    continue;
                }
                hitType = 2; // triangle
                hitIndex = k;
                hitDistance = f;
                this_t = f;
            }
        }
    }

    return Vector3(hitType, hitIndex, hitDistance);
}


bool checkIntersect (vector<Sphere> &sph, vector<Triangle> &tri, Ray &ray, 
    int which_object, int index, vector<vector<Vector4>> &venus) {
    
    float this_t = 1000;

    // for each sphere
    for(int k = 0; k<sph.size(); k++) {
        Sphere s = sph[k];
        Ray thisRay = ray;

        if (venus[k].size() > 0) {
            Matrix4 M = calculateMatrix(sph[k], venus, k);
            thisRay = calculateInvertRay(M, ray);
            s = Sphere(Vector3(0,0,0), sph[k].radius);
        }

        float f = s.intersect(thisRay);
        if (f != -1) {
            if (f < this_t) {
                if (which_object == 1 && index == k) {
                    continue;
                }
                this_t = f;
            }
        }
    }

    // for each triangle
    for(int k = 0; k< tri.size(); k++) {
        float f = tri[k].intersect(ray);
        if (f != -1) {
            if (f < this_t) {
                if (which_object == 2 && index == k) {
                    continue;
                }
                this_t = f;
            }
        }
    }

    if (this_t < 1000) {
      return true;
    }
    return false;
}

Color getShade (Vector3 objectP, vector<Sphere> &sph, vector<Triangle> &tri, Material m, Vector3 V, Vector3 n, 
  vector<Light>& dir_light, vector<Light>& point_light, int which_object, int index, vector<vector<Vector4>> &venus, bool reflect) {

    // normalize N
    n = n.normalize();

    // normalize view
    V = -V.normalize();

    // result
    Vector3 R = {0, 0, 0};

    // Accumulate directional lights: shade + shadow
    for (Light dl : dir_light) {
        Vector3 pos = -dl.position;
        Color color = dl.intensity;
        Vector3 I (color.r, color.g, color.b);

        Ray shadowRay(objectP, pos);
        
        // float closest_distance = 1000; // this needed fix
        bool intersect = false; 
        intersect = checkIntersect(sph, tri, shadowRay, which_object, index, venus);

        if (!intersect ) {
          // Calculate L
          Vector3 L = pos.normalize();

          // Calculate R
          Vector3 r = (-L + 2 * L.dot(n) * n);
          r = r.normalize();

          Vector3 shade = calculateShade(m, L, n, V, r, I);

          R += shade;
        } 

        if (!reflect) {
            R += (m.ka * I)/1.3;
        }
    }

    // Accumulate point lights: shade + shadow
    for (Light pl : point_light) {
        Vector3 pos = pl.position - objectP; // pl.position - point
        
        Color color;
        if (pl.falloff == 0) {
            color = pl.intensity;
        } else {
            float d = pl.position.distance(objectP);
            color = pl.intensity / pow(d, pl.falloff);
        }

        Vector3 I (color.r, color.g, color.b);

        Ray shadowRay(objectP, pos);
        
        // float closest_distance = 1000;
        bool intersect = false;
        intersect = checkIntersect(sph, tri, shadowRay, which_object, index, venus);

        if (!intersect ) {
          // Calculate L (may need to do point light calculation here)
          Vector3 L = (pos - n).normalize();
          // Calculate R
          Vector3 r = (-L + 2 * L.dot(n) * n);
          r = r.normalize();

          R += calculateShade(m, L, n, V, r, I); 
        }

        if (!reflect) {
            R += (m.ka * I)/1.3;
        }
    } 

    Color thisColor (R.x, R.y, R.z);
    return thisColor;
}


Color getReflection (Vector3 objectP, vector<Sphere> &sph, vector<Triangle> &tri, Material m, Vector3 V, Vector3 n, 
  vector<Light>& dir_light, vector<Light>& point_light, int which_object, int index, int recursiveDepth, vector<vector<Vector4>> &venus) {
    
    if (recursiveDepth == 0 || m.kr == Vector3(0, 0, 0)) {
        bool reflect = false;

        return getShade(objectP, sph, tri, m, V, n, 
                  dir_light, point_light, which_object, index, venus, reflect);

    } else {
        // get reflection ray
        Vector3 reflectionVec = (V - 2 *  (V.dot(n)) * n).normalize();
        Ray reflectionRay(objectP, reflectionVec);
        Ray closest_ray;

        // check intersect with other object
        Vector3 v = returnIntersect(sph, tri, reflectionRay, which_object, index, venus, closest_ray);
        float hitType = v[0];

        if (hitType != -1) {
            float hitIndex = v[1];
            float hitDistance = v[2];

            Vector3 hitNormal;
            Vector3 intersectPoint;
            Color reflectionColor;

            // ray hits sphere
            if (hitType == 1) {

                // if is ellipsoid
                if (venus[hitIndex].size() > 0) {
                    Matrix4 M = calculateMatrix(sph[hitIndex], venus, hitIndex);
                    Sphere s = Sphere(Vector3(0,0,0), sph[index].radius); // unit sphere

                    // compute point in object space (unit sphere's intersect point + transform)
                    intersectPoint = closest_ray.pointAt(hitDistance);

                    // compute normal (unit sphere's normal + transform)
                    hitNormal = s.getNormal(closest_ray, hitDistance);
                    hitNormal = M.invert().transpose() * hitNormal;

                } else {
                    // compute point in object space
                    intersectPoint = objectP + reflectionVec * hitDistance;

                    // compute normal
                    hitNormal = sph[hitIndex].getNormal(reflectionRay, hitDistance);
                }

                reflectionColor = getReflection(intersectPoint, sph, tri, sph[hitIndex].material, reflectionRay.dir, hitNormal, 
                      dir_light, point_light, hitType, hitIndex, recursiveDepth-1, venus);

            //ray hits triangle
            } else if (hitType == 2) {
                

                // compute point in object space
                intersectPoint = objectP + reflectionVec * hitDistance;

                // compute normal
                hitNormal = tri[hitIndex].getNormal();
                
                reflectionColor = getReflection(intersectPoint, sph, tri, tri[hitIndex].material, reflectionVec, hitNormal, 
                  dir_light, point_light, hitType, hitIndex, recursiveDepth-1, venus);
                
            }

            Vector3 temp = Vector3(reflectionColor[0], reflectionColor[1], reflectionColor[2]);
            temp = temp * m.kr;
            Color color(temp[0], temp[1], temp[2]);

            bool reflect = true;

            return getShade(objectP, sph, tri, m, V, n,
                      dir_light, point_light, which_object, index, venus, reflect) + color;

        
        } else {

            bool reflect = false;

            // return Color(0,0,0);
            return getShade(objectP, sph, tri, m, V, n, 
                  dir_light, point_light, which_object, index, venus, reflect);
        }
    }

};
