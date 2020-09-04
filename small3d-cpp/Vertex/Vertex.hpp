#pragma once

#include "../Vector/Vector.hpp"
#include "../utils.hpp"

class Vertex
{
public:
	Vector position;

	struct Color
	{
		float red;
		float green;
		float blue;
	} color;

	struct TexCoord
	{
		float u;
		float v;
	} texCoord;

	float rhw;

	Vertex();
	~Vertex();

	Vertex(const Vector & pos, const Color & col, const TexCoord & tc, float _rhw);

	Vertex operator+(const Vertex & val) const;
	void operator+=(const Vertex & val);
	Vertex operator-(const Vertex & val) const;
	
	Vertex Division(const Vertex & val, float w) const;
	void RHWInit();
	void Lerp(const Vertex & a, const Vertex & b, float t);

};