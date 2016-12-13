#include "Shape.h"
#include "Colors.h"
#include "Matrices.h"
#include "Vectors.h"
#include "Camera.h"
#include "Light.h"
#include "Boundary.h"
// #include "CImg/CImg.h"
#include "Shader.cpp"
// #include "Matrices.cpp"
#include "objReader.cpp"

#include <vector>
#include <string>
#include <cmath>
#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
// using namespace cimg_library;

#define PI 3.14159265
#define RECURSIVE_DEPTH 5

int WIDTH = 750;
int HEIGHT = 750;
string filename;
Vector3 bound_max(0, 0, 0);
Vector3 bound_min(1000, 1000, 1000);

/**
 * init
 * - basically check intersect and shade things
 * 
 * @param camera      [description]
 * @param sph         [description]
 * @param tri         [description]
 * @param point_light [description]
 * @param dir_light   [description]
 * @param amb         [description]
 * @param venus       [description]
 */
void init(Camera &camera, vector<Sphere> &sph, vector<Triangle> &tri, vector<Light> &point_light, vector<Light> &dir_light, vector<Color> &amb,
 vector<vector<Vector4>> &venus) {

    // background color is the sum of all ambient colors
    Color background; //default is black
    for(int i=0; i<amb.size(); i++) {
        background += amb[i];
    }

    Mat output_image (HEIGHT, WIDTH, DataType<Vec3b>::type);

    //// ---------- CIMG -----------
    // CImg<float> output_image (HEIGHT, WIDTH, 1, 3); 
    //// ---------- CIMG -----------

    for(int i = 0; i < WIDTH; i++) {
        for(int j = 0; j < HEIGHT; j++) {

            // ==========================
            // ========= Set Up =========
            // ==========================
            // final color initialization
            Color final_color = background;
            // for each pixel compute viewing ray
            float u = 1 - i / (float) WIDTH;
            float v = j / (float) HEIGHT;
            Vector3 P = u * (camera.ll*v + camera.ul*(1-v)) + (1-u) * (camera.lr*v + camera.ur*(1-v));
            Ray ray(camera.eye, P - camera.eye);

 
            // =========================
            // ==== Check Intersect ====
            // =========================           
            // https://www.cs.utah.edu/~shirley/books/fcg2/rt.pdf
            float closest_distance = 1000;
            int which_object = 0; // 0 = no object, 1 sphere, 2 triangle
            int index = -1;

            // for each sphere
            Ray closest_sphere_ray;
            for(int k =0; k<sph.size(); k++) {

                Sphere thisSphere = sph[k];
                Ray thisRay = ray;

                // if this sphere has transformation, use unit sphere and invert ray
                if (venus[k].size() > 0) {
                    // compute rotation matrix
                    Matrix4 M = calculateMatrix(sph[k], venus, k);
                    // compute invert transformed ray
                    thisRay = calculateInvertRay(M, ray);
                    // unit sphere
                    thisSphere = Sphere(Vector3(0,0,0), sph[k].radius); 
                } 

                // find closest intersect
                float f = thisSphere.intersect(thisRay);
                if (f != -1) {
                    if (f < closest_distance) {
                        closest_distance = f;
                        which_object = 1;
                        index = k;
                        closest_sphere_ray = thisRay;
                    }
                }
            }

            // for each triangle
            for(int k =0; k< tri.size(); k++) {
                // find closest intersect
                float f = tri[k].intersect(ray);
                if (f != -1) {
                    if (f < closest_distance) {
                        closest_distance = f;
                        which_object = 2;
                        index = k;
                    }
                }
            }

            // =========================
            // ======== Shading ========
            // =========================
            Vector3 currNormal; Material currMaterial; Vector3 objectP; 

            // shading if is sphere
            if (which_object == 1) {

                currMaterial = sph[index].material;

                // if there is transformation on this sphere, 
                // find point and normal on unit sphere, and transform both
                if (venus[index].size() > 0) {
                    // get transform matrix
                    Matrix4 M = calculateMatrix(sph[index], venus, index);
                    // Ray thisRay = calculateInvertRay(M, ray);

                    // compute point in object space (unit sphere's intersect point + transform)
                    Sphere s = Sphere(Vector3(0,0,0), sph[index].radius); // unit sphere
                    Vector3 objectP = closest_sphere_ray.pointAt(closest_distance);
                    // // Vector3 objectP = closest_sphere_ray.pointAt(closest_distance);
                    // // objectP = M * objectP;
                    
                    // compute normal (unit sphere's normal + transform)
                    currNormal = s.getNormal(closest_sphere_ray, closest_distance);
                    
                    // currNormal = M * currNormal;

                    Vector4 temp(currNormal[0], currNormal[1], currNormal[2], 1);
                    temp = M.invert().transpose() * temp;
                    currNormal = Vector3(temp[0], temp[1], temp[2]);
                    // currNormal.normalize();

                // else just do basic sphere point and normal
                } else {
                    // compute point in object space
                    objectP = ray.orig + closest_distance*(ray.dir);
                    // compute normal
                    currNormal = sph[index].getNormal(closest_sphere_ray, closest_distance);
                }
                final_color += getReflection(objectP, sph, tri, currMaterial, ray.dir, currNormal, 
                    dir_light, point_light, which_object, index, RECURSIVE_DEPTH, venus);

            // shading if is triangle
            } else if (which_object == 2) {
                
                currMaterial = tri[index].material;
                
                // compute point in object space
                objectP = ray.orig + closest_distance*(ray.dir);
                // compute normal
                currNormal = tri[index].getNormal();

                final_color += getReflection(objectP, sph, tri, currMaterial, ray.dir, currNormal, 
                    dir_light, point_light, which_object, index, RECURSIVE_DEPTH, venus);

            } else {

                final_color = background;

            }

            // =========================
            // == Final Color Convert ==
            // =========================
            final_color.r = int(MIN(1.0, final_color.r) * 255);
            final_color.g = int(MIN(1.0, final_color.g) * 255);
            final_color.b = int(MIN(1.0, final_color.b) * 255);

            output_image.at<Vec3b>(j, i) = Vec3b(final_color.b, final_color.g, final_color.r);

            //// ---------- CIMG -----------
            // const float color[] = {final_color.r, final_color.g, final_color.b};
            // output_image.draw_point(i, j, color);
            //// ---------- CIMG -----------
        }
    }

    imwrite("../output/" + filename +  "_output.jpg", output_image);
    namedWindow("Output image", CV_WINDOW_AUTOSIZE);
    resizeWindow("Output image", WIDTH, HEIGHT);
    imshow("Output image", output_image);

    cout << "bound max: " << bound_max << endl;
    cout << "bound min: " << bound_min << endl;

    //// ---------- CIMG -----------
    // CImgDisplay local(output_image, "Output Image", 0);
    // const char * output_filename = ("../output/" + filename +  "_output.ppm").c_str();
    // output_image.save(output_filename);
    //// ---------- CIMG -----------
    waitKey(0);
}

/**
 * main
 * - fileIO handling
 * 
 * @param  argc [description]
 * @param  argv [description]
 * @return      [description]
 */
int main(int argc, char *argv[]) {
    // Check arg
    if ( argc != 2 ){
        printf("usage: ./as2 <Input path>\n");
        return -1;
    }

    string line;
    string delimiter = " ";

    Camera camera;
    vector<Sphere> sph;
    vector<Triangle> tri;
    vector<Color> amb;
    vector<Light> point_light;
    vector<Light> dir_light;

    vector<vector<Vector4>> Venus;
    
    int object = 0;
    int next_tri = -1;

    ifstream infile;
    filename = argv[1];
    infile.open(filename, ios::in);

    // strip filename out for output
    filename = filename.substr(filename.find_last_of("\\/"), -1);
    filename = filename.substr(0, filename.find_last_of("."));

    if (infile.is_open()) {
        while (getline(infile, line)) {
            vector<string> v;
            int pos = 0;
            string token;

            while ((pos = line.find(delimiter)) != string::npos) {
                token = line.substr(0, pos);
                v.push_back(token);
                line.erase(0, pos + delimiter.length());
            }
            v.push_back(line);

            if (v[0].compare("cam") == 0) {
                Vector3 eye(stof(v[1]), stof(v[2]), stof(v[3]));
                Vector3 ll(stof(v[4]), stof(v[5]), stof(v[6]));
                Vector3 lr(stof(v[7]), stof(v[8]), stof(v[9]));
                Vector3 ul(stof(v[10]), stof(v[11]), stof(v[12]));
                Vector3 ur(stof(v[13]), stof(v[14]), stof(v[15]));

                camera = Camera(eye, ll, lr, ul, ur);

            } else if (v[0].compare("sph") == 0) {
                Vector3 s(stof(v[1]), stof(v[2]), stof(v[3]));
                float radius = stof(v[4]);

                Sphere sip (s, radius);
                sph.push_back(sip);
                Venus.push_back(vector<Vector4>());

                Vector3 thisMin = sip.bounds.mmin;
                Vector3 thisMax = sip.bounds.mmax;

                bound_min = Vector3(min(bound_min.x, thisMin.x),
                    min(bound_min.y, thisMin.y),
                    min(bound_min.z, thisMin.z));

                bound_max = Vector3(max(bound_max.x, thisMax.x),
                    max(bound_max.y, thisMax.y),
                    max(bound_max.z, thisMax.z));

                object = 1;

            } else if (v[0].compare("tri") == 0) {
                Vector3 a(stof(v[1]), stof(v[2]), stof(v[3]));
                Vector3 b(stof(v[4]), stof(v[5]), stof(v[6]));
                Vector3 c(stof(v[7]), stof(v[8]), stof(v[9]));

                Triangle tip (a, b, c);
                tri.push_back(tip);

                Vector3 thisMin = tip.bounds.mmin;
                Vector3 thisMax = tip.bounds.mmax;

                bound_min = Vector3(min(bound_min.x, thisMin.x),
                    min(bound_min.y, thisMin.y),
                    min(bound_min.z, thisMin.z));

                bound_max = Vector3(max(bound_max.x, thisMax.x),
                    max(bound_max.y, thisMax.y),
                    max(bound_max.z, thisMax.z));

                object = 2;

            } else if (v[0].compare("mat") == 0) {
                Vector3 ka(stof(v[1]), stof(v[2]), stof(v[3]));
                Vector3 kd(stof(v[4]), stof(v[5]), stof(v[6]));
                Vector4 ks(stof(v[7]), stof(v[8]), stof(v[9]), stof(v[10]));
                Vector3 kr(stof(v[11]), stof(v[12]), stof(v[13]));

                Material mate(ka, kd, ks, kr);

                if (object == 1) {
                    Sphere sip = sph[sph.size()-1];
                    sph.pop_back();
                    sph.push_back(Sphere(sip.center, sip.radius, mate));

                } else if (object == 2) {
                    Triangle tip = tri[tri.size()-1];
                    tri.pop_back();
                    tri.push_back(Triangle(tip.a, tip.b, tip.c, mate));

                } else if (object == 3) {
                    while (next_tri < tri.size()) {
                        cout << "triangle material adding: " << next_tri << endl;
                        Triangle tip = tri[next_tri];
                        cout << "triangle adding: " << tip << endl;
                        tip.material = mate;
                        tri[next_tri] = tip;
                        next_tri += 1;
                    }
                }

                object = 0;

            } else if (v[0].compare("obj") == 0) {
                string filename = v[1];

                cout << filename << endl;
                vector<Triangle> objTris = readObj(filename);

                next_tri = tri.size();

                for (Triangle objTri: objTris) {
                    tri.push_back(objTri);
                }

                object = 3;

            } else if (v[0].compare("ltp") == 0) {
                Vector3 pl(stof(v[1]), stof(v[2]), stof(v[3]));
                Color I_pl(stof(v[4]), stof(v[5]), stof(v[6]));
                int falloff = 0;
                try {
                    falloff = stoi(v[7]);
                } catch (exception &e) {
                    falloff = 0;
                }

                point_light.push_back(Light(pl, I_pl, falloff));

            } else if (v[0].compare("ltd") == 0) {
                Vector3 dl(stof(v[1]), stof(v[2]), stof(v[3]));
                Color I_dl(stof(v[4]), stof(v[5]), stof(v[6]));

                dir_light.push_back(Light(dl, I_dl, 0));

            } else if (v[0].compare("lta") == 0) {
                Color lta(stof(v[1]), stof(v[2]), stof(v[3]));

                amb.push_back(lta);

            } else if (v[0].compare("xft") == 0) {
                Vector4 xform4(0, stof(v[1]), stof(v[2]), stof(v[3]));

                Venus[Venus.size()-1].push_back(xform4);

            } else if (v[0].compare("xfr") == 0) {
                Vector4 rot4(1, stof(v[1]), stof(v[2]), stof(v[3]));

                Venus[Venus.size()-1].push_back(rot4);

            } else if (v[0].compare("xfs") == 0) {
                Vector4 sca4(2, stof(v[1]), stof(v[2]), stof(v[3]));

                Venus[Venus.size()-1].push_back(sca4);

            } else if (v[0].compare("xfz") == 0) {
                // resets transformation to identity
                continue;
            } else if (v[0].compare("lineupsph") == 0) {
                int numbersph = stof(v[1]);

                Sphere lastSph = sph[sph.size()-1];
                Vector3 lastCenter = lastSph.center;
                Material lastM = lastSph.material;
                float lastRadius = lastSph.radius;
                float margin = lastRadius*3;

                for (int i = 0; i < numbersph; i++) {
                    Vector3 nextCenter(lastCenter.x + margin, lastCenter.y, lastCenter.z);
                    lastCenter = nextCenter;
                    Sphere sip (nextCenter, lastRadius, lastM);
                    sph.push_back(sip);
                    Venus.push_back(vector<Vector4>());
                }

            } else {
                cerr << "Could not read line." << endl;
            }

        }
        infile.close();

    } else {
        cout << "unable to open file";
    }

    init(camera, sph, tri, point_light, dir_light, amb, Venus);

    return 0;
}
