#pragma once
#include <cmath>

using namespace std;

struct Vector {
	// if w = 0, vector is a direction
	// if w = 1, vector is a position
	double x, y, z, w;


	double& operator[](int index);
	double operator[](int index) const;
	bool operator==(const Vector& v) const;
	Vector operator*(double a) const;
	Vector operator*(const Vector& v) const;
	Vector operator/(double d) const;
	Vector operator+(const Vector& v) const;
	Vector operator-() const;
	Vector operator-(const Vector& v) const;
	double sum() const;
	double dot(const Vector& v) const;
	double mag() const;
	Vector norm() const;
	Vector cross(const Vector& v) const;
	bool isClose(const Vector& v) const;

	friend std::ostream& operator<<(std::ostream& os, const Vector& V);
};


inline std::ostream& operator<<(std::ostream& os, const Vector& V) {
    os << "(" << V.x << ", " << V.y << ", " << V.z << ", " << V.w << ")";
    return os;
}

Vector operator*(double a, const Vector& v) {
	return {a * v.x, a * v.y, a * v.z, v.w};
}

inline double& Vector::operator[](int index) {
	return (&x)[index];
}

inline double Vector::operator[](int index) const {
	return (&x)[index];
}

inline bool Vector::operator==(const Vector& v) const {
	return x==v.x && y==v.y && z==v.z && w==v.w;
}

inline Vector Vector::operator*(double a) const {
	return {a * x, a * y, a * z, w};
}

inline Vector Vector::operator*(const Vector& v) const {
	return {x * v.x, y * v.y, z * v.z, w * v.w};
}

inline Vector Vector::operator/(double d) const {
	return {x/d, y/d, z/d, w};
}

inline Vector Vector::operator+(const Vector& v) const {
	return {x + v.x, y + v.y, z + v.z, w + v.w};
}

inline Vector Vector::operator-() const {
	return {-x, -y, -z, w};
}

inline Vector Vector::operator-(const Vector& v) const {
	return {x - v.x, y - v.y, z - v.z, double(bool(w) != bool(v.w))};
}

inline double Vector::sum() const {
	return x + y + z;
}

inline double Vector::dot(const Vector& v) const {
	return (*this * v).sum();
}

inline double Vector::mag() const {
	return sqrt(x*x + y*y + z*z);
}

inline Vector Vector::norm() const {
	return *this / this->mag();
}

inline Vector Vector::cross(const Vector& v) const {
	return {
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x,
		w
	};
}

inline bool Vector::isClose(const Vector& v) const {
	double thresh = 0.0005;
	bool dx = abs(x - v.x) < thresh;
	bool dy = abs(y - v.y) < thresh;
	bool dz = abs(z - v.z) < thresh;
	if (dx && dy && dz) {
		return true;
	}
	return false;
}