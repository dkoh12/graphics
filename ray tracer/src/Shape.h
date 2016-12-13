#ifndef SHAPE_H
#define SHAPE_H

#include <cmath>
#include <stdlib.h>
#include "Vectors.h"
#include <math.h>
#include <algorithm>
#include "Material.h"
#include "Boundary.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

struct Material;
// struct Intersection;
struct Boundaries;

struct Shape {
    // virtual Intersection intersect(Ray &ray) = 0;

    virtual Boundaries getBoundary() = 0;
};

///////////////////////////////////////////////////////////////////////////////
// Sphere
///////////////////////////////////////////////////////////////////////////////
struct Sphere : Shape
{
    Vector3 center; // center
    float radius; // radius
    Material material;
    Boundaries bounds;


    Sphere(Vector3 c, float r) : center(c), radius(r) {
        bounds.mmin = center - Vector3(radius, radius, radius);
        bounds.mmax = center + Vector3(radius, radius, radius);
    };

    Sphere(Vector3 c, float r, Material m) : center(c), radius(r), material(m) {
        bounds.mmin = center - Vector3(radius, radius, radius);
        bounds.mmax = center + Vector3(radius, radius, radius);
    }

    // functions
    // bool intersect(const Ray &ray, const float &t);
    float intersect(Ray &ray);
    Vector3 getNormal(Ray &ray, float distance);

    // print statements for debugging purposes
    friend std::ostream& operator<<(std::ostream& os, const Sphere& sph);

    // virtual Intersection intersect(Ray &ray);
    virtual Boundaries getBoundary();
    Color getColor(Vector3);
};

///////////////////////////////////////////////////////////////////////////////
// Triangle
///////////////////////////////////////////////////////////////////////////////
struct Triangle : Shape
{
    Vector3 a, b, c;
    Material material;
    Boundaries bounds;


    Triangle(Vector3 a_, Vector3 b_, Vector3 c_) : a(a_), b(b_), c(c_) {
        bounds.mmin = Vector3(min(a.x, min(b.x, c.x)),
                            min(a.y, min(b.y, c.y)),
                            min(a.z, min(b.z, c.z)));

        bounds.mmax = Vector3(max(a.x, max(b.x, c.x)),
                            max(a.y, max(b.y, c.y)),
                            max(a.z, max(b.z, c.z)));
    };

    // ctors
    Triangle(Vector3 _a, Vector3 _b, Vector3 _c, Material _m) : a(_a), b(_b), c(_c), material(_m) {
        bounds.mmin = Vector3(min(a.x, min(b.x, c.x)),
                            min(a.y, min(b.y, c.y)),
                            min(a.z, min(b.z, c.z)));

        bounds.mmax = Vector3(max(a.x, max(b.x, c.x)),
                            max(a.y, max(b.y, c.y)),
                            max(a.z, max(b.z, c.z)));
    };

    // functions
    float intersect(Ray &ray);
    Vector3 getNormal();

    friend std::ostream& operator<<(std::ostream& os, const Triangle &tri);


    // virtual Intersection intersect(Ray &ray);
    virtual Boundaries getBoundary();
    Color getColor(Vector3);

};


///////////////////////////////////////////////////////////////////////////////
// inline functions for Spheres
///////////////////////////////////////////////////////////////////////////////

// print statements for debugging purposes
inline std::ostream& operator<<(std::ostream& os, const Sphere& sph) {
    os << "(" << sph.center << ", " << sph.radius << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Triangle& tri) {
    os << "(" << tri.a << ", " << tri.b << ", " << tri.c << ")";
    return os;
}

inline float Sphere::intersect(Ray &ray) {
    // Vector3 d = center - ray.orig;
    Vector3 d = ray.orig - center; // delta

    // (x-x_)^2 + (y-y_)^2 + (z-z_)^2 - r^2 =0
    // (p-c) * (p-c) - r^2 = 0
    // (e + td - c) * (e + td - c) - r^2 = 0
    // d*d t^2 + 2d(e-c)t +(e-c)(e-c)-r^2 =0

    float a = ray.dir.dot(ray.dir);
    float b = 2 * d.dot(ray.dir);
    float c = d.dot(d) - (radius * radius);

    float disc = b * b - 4 * a * c; // discriminant

    // cout << "a " << a << " b " << b << " c " << c << " disc " << disc << endl;

    if (disc < 0) {
        // cout << "disc < 0" << endl;
        return -1; // no intersection
    }

    // disc = 0; 1 solution.
    // disc > 0; 2 solutions

    disc = sqrt(disc);

    float q;
    if (b < 0) {
        q = (-b - disc) * 0.5;
    } else {
        q = (-b + disc) * 0.5;
    }

    float r1 = q/a;
    float r2 = c/q;

    // pick smaller distance
    if (r1 > r2) {
        float tmp = r1;
        r1 = r2;
        r2 = tmp;
    }

    float distance = r1; //find nearest intersection
    if (distance < 0) { //object cant be behind camera
        distance = r2;
    }

    if (distance < 0 || isnan(distance)) {
        // cout << "distance < 0 or isnan" << endl;
        return -1; // no intersection
    }

    return distance;
}

inline Vector3 Sphere::getNormal(Ray &ray, float distance) {
    Vector3 point = ray.orig + (ray.dir * distance); // point on sphere

    Vector3 normal = (point-center).normalize(); // surface normal of sphere

    return normal;
}

inline Boundaries Sphere::getBoundary() {
    return bounds;
}

///////////////////////////////////////////////////////////////////////////////
// inline functions for Triangle
///////////////////////////////////////////////////////////////////////////////

inline float Triangle::intersect(Ray &ray) {
    Vector3 e1, e2, h, s, q, normal;
    float t, f, u, v, distance;

    e1 = b - a; 
    e2 = c - a; 

    h = ray.dir.cross(e2);
    t = e1.dot(h);

    if (t > -0.00001 && t < 0.00001)
        return -1; // no intersection

    f = 1 / t;
    s = ray.orig - a;

    u = f * s.dot(h);

    if (u < 0 || u >1)
        return -1; // no intersection

    q = s.cross(e1);
    v = f * ray.dir.dot(q);

    if (v < 0 || u+v > 1)
        return -1; // no intersection

    distance = f * e2.dot(q);
    
    if (distance > 0.00001) { // ray intersection
        return distance;
    } else {
        return -1; // line intersection but no ray intersection
    }

}

inline Vector3 Triangle::getNormal() {
    Vector3 e1, e2, normal;
    
    e1 = b - a;
    e2 = c - a;
    
    // shading of triangle is not dependent on normal
    normal = (e1.cross(e2)).normalize();

    return normal;
}


inline Boundaries Triangle::getBoundary() {
    return bounds;
}


#endif



