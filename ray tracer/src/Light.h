#ifndef LIGHT_H
#define LIGHT_H

#include "Vectors.h"
#include "Colors.h"
#include <math.h>
#include <cmath>

struct Light {
	Vector3 position;
	Color intensity;
	int falloff;

	Light() {};

	// Light(Vector3 pos, Colors inten) : position(pos), colors(inten) {};
	Light(Vector3 pos, Color color, int falloff_) : position(pos), intensity(color), falloff(falloff_) {};

	Color intensityAt(Vector3 &point);

};

inline Color Light::intensityAt(Vector3 &point) {
	if (falloff == 0) {
		return intensity;
	} else {
		float d = position.distance(point);
		return intensity / pow(d, falloff);
	}
}


#endif