#include "Transform.hpp"

Transform::Transform()
	: height(0.0f), width(0.0f)
{}

Transform::Transform(float h, float w)
{
	height = h;
	width = w;
	float aspect = w / h;
	world.SetIdentity();
	view.SetIdentity();
	projection.SetPerspective(3.1415926f * 0.5f, aspect, 1.0f, 500.0f);
	Update();
}

void Transform::Update()
{
	transform = (world * view) * projection;
}

Vector Transform::Apply(const Vector & vec) const
{
	return Matrix::Apply(vec, transform);
}

Vector Transform::Homogenize(const Vector & vec) const
{
	Vector result;
	float rhw = 1.0f / vec.w;
	result.x = (vec.x * rhw + 1.0f) * width * 0.5f;
	result.y = (1.0f - vec.y * rhw) * height * 0.5f;
	result.z = vec.z * rhw;
	result.w = 1.0f;
	return result;
}