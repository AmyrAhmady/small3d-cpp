#include "Vector.hpp"

Vector::Vector()
	: x(.0f), y(.0f), z(.0f), w(.0f)
{}

Vector::Vector(float x, float y, float z)
	: x(x), y(y), z(z), w(.0f)
{}

Vector::Vector(float x, float y, float z, float w)
	: x(x), y(y), z(z), w(w)
{}

Vector Vector::operator+(const Vector & val) const
{
	return Vector(x + val.x, y + val.y, z + val.z, 1.0f);
}

void Vector::operator+=(const Vector & val)
{
	x += val.x;
	y += val.y;
	z += val.z;
	w = 1.0f;
}

Vector Vector::operator-(const Vector & val) const
{
	return Vector(x - val.x, y - val.y, z - val.z, 1.0f);
}

Vector Vector::operator*(const Vector & val) const
{
	return CrossProduct(*this, val);
}

Vector Vector::operator*(float val) const
{
	return Vector(x * val, y * val, z * val);
}

float Vector::DotProduct(const Vector & a, const Vector & b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vector Vector::CrossProduct(const Vector & a, const Vector & b)
{
	float m1, m2, m3;
	m1 = a.y * b.z - a.z * b.y;
	m2 = a.z * b.x - a.x * b.z;
	m3 = a.x * b.y - a.y * b.x;
	return Vector(m1, m2, m3, 1.0f);
}

float Vector::Length() const
{
	return static_cast<float>(sqrt((x * x) + (y * y) + (z * z)));
}

void Vector::Lerp(const Vector & a, const Vector & b, float t)
{

	x = lerp(a.x, b.x, t);
	y = lerp(a.y, b.y, t);
	z = lerp(a.z, b.z, t);
	w = 1.0f;
}

void Vector::Normalise()
{
	float length = Length();
	if (length != 0.0f) {
		float inv = 1.0f / length;
		x *= inv;
		y *= inv;
		z *= inv;
	}
}