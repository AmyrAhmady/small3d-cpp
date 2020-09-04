#pragma once

#include "../Matrix/Matrix.hpp"

class Transform
{
public:
	float width;
	float height;
	Matrix world;        
	Matrix view;
	Matrix projection;
	Matrix transform;

	Transform();
	Transform(float h, float w);

	~Transform() {}

	void Update();
	Vector Apply(const Vector & vec) const;
	Vector Homogenize(const Vector & vec) const;

};