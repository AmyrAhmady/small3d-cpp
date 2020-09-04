#pragma once

#include <array>
#include "../Vector/Vector.hpp"

class Matrix
{
private:
	std::array<std::array<float, 4>, 4> 
		mat;

public:
	Matrix();
	Matrix(const std::array<std::array<float, 4>, 4> & val);

	~Matrix() {}


	Matrix operator+(const Matrix & val) const;
	Matrix operator-(const Matrix & val) const;
	Matrix operator*(const Matrix & val) const;
	Matrix operator*(float val) const;

	void Fill(float value);

	void Erase();

	void SetIdentity();
	void SetTranslate(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotate(float x, float y, float z, float theta);
	void SetPerspective(float fovY, float aspectRatio, float zNear, float zFar);
	void SetLookAt(const Vector & eye, const Vector & at, const Vector & up);

	static Vector Apply(const Vector & vec, const Matrix & mat);

	std::array<std::array<float, 4>, 4> &
		Value() { return mat; }

	std::array<std::array<float, 4>, 4>
		Value() const { return mat; }

	std::array<std::array<float, 4>, 4>
		Value(const std::array<std::array<float, 4>, 4> & val) 
	{
		mat = val;
		return mat;
	}

};
