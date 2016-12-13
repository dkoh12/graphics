#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vectors.h"
#include "Colors.h"
#include <stdlib.h>

struct Material {
	Vector3 ka;
	Vector3 kd;
	Vector4 ks;
	Vector3 kr;

	Material() {}

	Material(Vector3 &ka_, Vector3 &kd_, Vector4 &ks_, Vector3 &kr_) :
	ka(ka_), kd(kd_), ks(ks_), kr(kr_) {}

	friend std::ostream& operator<<(std::ostream& os, const Material& mat);

};

inline std::ostream& operator<<(std::ostream& os, const Material& mat) {
    os << "(" << mat.ka << ", " <<  mat.kd << ", " << mat.ks << ", " <<  mat.kr << ")";
    return os;
}

#endif

