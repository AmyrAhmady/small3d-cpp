#include "Matrix.hpp"

Matrix::Matrix()
	: mat(std::array<std::array<float, 4>, 4>())
{}

Matrix::Matrix(const std::array<std::array<float, 4>, 4> & val)
	: mat(val)
{}

Matrix Matrix::operator+(const Matrix & val) const
{
	const auto rightMat = val.Value();
	Matrix result;
	std::array<std::array<float, 4>, 4> resultMat;

	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++)
			resultMat[i][j] = mat[i][j] + rightMat[i][j];
	}
	
	result.Value(resultMat);
	return result;
}

Matrix Matrix::operator-(const Matrix & val) const
{
	const auto & rightMat = val.Value();
	Matrix result;
	std::array<std::array<float, 4>, 4> resultMat;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			resultMat[i][j] = mat[i][j] - rightMat[i][j];
	}

	result.Value(resultMat);
	return result;
}

Matrix Matrix::operator*(const Matrix & val) const
{
	const auto & rightMat = val.Value();
	Matrix result;
	std::array<std::array<float, 4>, 4> resultMat;

	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			resultMat[j][i] = 
				(mat[j][0] * rightMat[0][i]) +
				(mat[j][1] * rightMat[1][i]) +
				(mat[j][2] * rightMat[2][i]) +
				(mat[j][3] * rightMat[3][i]);
		}
	}

	result.Value(resultMat);
	return result;
}

Matrix Matrix::operator*(float val) const
{
	Matrix result;
	std::array<std::array<float, 4>, 4> resultMat;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			resultMat[i][j] = mat[i][j] * val;
	}

	result.Value(resultMat);
	return result;
}

void Matrix::Fill(float value)
{
	auto temp = std::array<float, 4>();
	temp.fill(value);
	mat.fill(temp);
}

void Matrix::Erase()
{
	Fill(.0f);
}

void Matrix::SetIdentity()
{
	Fill(.0f);
	mat[0][0] = 1.0f;
	mat[1][1] = 1.0f;
	mat[2][2] = 1.0f;
	mat[3][3] = 1.0f;
}

void Matrix::SetTranslate(float x, float y, float z)
{
	SetIdentity();
	mat[3][0] = x;
	mat[3][1] = y;
	mat[3][2] = z;
}

void Matrix::SetScale(float x, float y, float z)
{
	SetIdentity();
	mat[0][0] = x;
	mat[1][1] = y;
	mat[2][2] = z;
}

void Matrix::SetRotate(float x, float y, float z, float theta)
{
	float qsin = static_cast<float>(sin(theta * 0.5f));
	float qcos = static_cast<float>(cos(theta * 0.5f));
	Vector vec(x, y, z, 1.0f);
	float w = qcos;
	vec.Normalise();
	x = vec.x * qsin;
	y = vec.y * qsin;
	z = vec.z * qsin;
	mat[0][0] = 1 - 2 * y * y - 2 * z * z;
	mat[1][0] = 2 * x * y - 2 * w * z;
	mat[2][0] = 2 * x * z + 2 * w * y;
	mat[0][1] = 2 * x * y + 2 * w * z;
	mat[1][1] = 1 - 2 * x * x - 2 * z * z;
	mat[2][1] = 2 * y * z - 2 * w * x;
	mat[0][2] = 2 * x * z - 2 * w * y;
	mat[1][2] = 2 * y * z + 2 * w * x;
	mat[2][2] = 1 - 2 * x * x - 2 * y * y;
	mat[0][3] = mat[1][3] = mat[2][3] = 0.0f;
	mat[3][0] = mat[3][1] = mat[3][2] = 0.0f;
	mat[3][3] = 1.0f;
}

void Matrix::SetPerspective(float fovY, float aspectRatio, float zNear, float zFar)
{
	float scale = 
		1.0f / static_cast<float>(tan(fovY * 0.5f));

	Erase();

	mat[0][0] = scale / aspectRatio;
	mat[1][1] = scale;
	mat[2][2] = zFar / (zFar - zNear);
	mat[3][2] = -zNear * zFar / (zFar - zNear);
	mat[2][3] = 1;
}

void Matrix::SetLookAt(const Vector & eye, const Vector & at, const Vector & up)
{
	Vector xaxis, yaxis, zaxis;

	zaxis = at - eye;
	zaxis.Normalise();

	xaxis = up * zaxis; // Vector::CrossProduct
	xaxis.Normalise();

	yaxis = zaxis * xaxis;  // Vector::CrossProduct

	mat[0][0] = xaxis.x;
	mat[1][0] = xaxis.y;
	mat[2][0] = xaxis.z;
	mat[3][0] = -Vector::DotProduct(xaxis, eye);

	mat[0][1] = yaxis.x;
	mat[1][1] = yaxis.y;
	mat[2][1] = yaxis.z;
	mat[3][1] = -Vector::DotProduct(yaxis, eye);

	mat[0][2] = zaxis.x;
	mat[1][2] = zaxis.y;
	mat[2][2] = zaxis.z;
	mat[3][2] = -Vector::DotProduct(zaxis, eye);

	mat[0][3] = mat[1][3] = mat[2][3] = 0.0f;
	mat[3][3] = 1.0f;
}

Vector Matrix::Apply(const Vector & vec, const Matrix & matrix)
{
	Vector result;
	const auto & mat = matrix.Value();
	result.x = vec.x * mat[0][0] + vec.y * mat[1][0] + vec.z * mat[2][0] + vec.w * mat[3][0];
	result.y = vec.x * mat[0][1] + vec.y * mat[1][1] + vec.z * mat[2][1] + vec.w * mat[3][1];
	result.z = vec.x * mat[0][2] + vec.y * mat[1][2] + vec.z * mat[2][2] + vec.w * mat[3][2];
	result.w = vec.x * mat[0][3] + vec.y * mat[1][3] + vec.z * mat[2][3] + vec.w * mat[3][3];
	return result;
}