#pragma once

#include <cmath>
#include "../utils.hpp"

class Vector
{
public:
	float x;
	float y;
	float z;
	float w;

	Vector();
	Vector(float x, float y, float z);
	Vector(float x, float y, float z, float w);

	~Vector() {}

	Vector operator+(const Vector & val) const;
	void operator+=(const Vector & val);
	Vector operator-(const Vector & val) const;
	Vector operator*(const Vector & val) const;
	Vector operator*(float val) const;

	static float DotProduct(const Vector & a, const Vector & b);
	static Vector CrossProduct(const Vector & a, const Vector & b);

	float Length() const;
	void Lerp(const Vector & a, const Vector & b, float t);
	void Normalise();
};