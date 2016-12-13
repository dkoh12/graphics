#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "Vectors.h"
#include <algorithm>

struct Boundaries {
	Vector3 mmin, mmax;

	Boundaries() : mmin(Vector3(0,0,0)), mmax(Vector3(0,0,0)) {};

	float splitValue(char axis);
	bool intersect(const Ray& ray, float *dist);
};

// returns center value for given axis
inline float Boundaries::splitValue(char axis) {
	switch (axis) {
		case 'x':
			return (mmin.x + mmax.x) / 2;
		case 'y':
			return (mmin.y + mmax.y) / 2;
		case 'z':
			return (mmin.z + mmax.z) / 2;
		default:
			return 0;
	}
}

// ray AABB intersection
// Adapted from: http://gamedev.stackexchange.com/a/18459
inline bool Boundaries::intersect(const Ray& ray, float *dist) {

	float t1 = (mmin.x - ray.orig.x) * ray.fracDir.x;
	float t2 = (mmax.x - ray.orig.x) * ray.fracDir.x;
	float t3 = (mmin.y - ray.orig.y) * ray.fracDir.y;
	float t4 = (mmax.y - ray.orig.y) * ray.fracDir.y;
	float t5 = (mmin.z - ray.orig.z) * ray.fracDir.z;
	float t6 = (mmax.z - ray.orig.z) * ray.fracDir.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	// tmax < 0 = ray intersects AABB but AABB is behind us
	// tmin > tmax = ray doesn't intersect AABB
	if (tmax < 0 || tmin > tmax)
		return false;

	return true;

}

#endif