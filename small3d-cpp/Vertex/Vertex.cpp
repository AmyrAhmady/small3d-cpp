#include "Vertex.hpp"

Vertex::Vertex() :
	position({ 0.0f, 0.0f ,0.0f, 0.0f }), color({ 0.0f, 0.0f, 0.0f }), texCoord({ 0.0f, 0.0f }), rhw(0.0f)
{}

Vertex::~Vertex()
{}

Vertex::Vertex(const Vector & pos, const Color & col, const TexCoord & tc, float _rhw) :
	position(pos), color(col), texCoord(tc), rhw(_rhw)
{}

Vertex Vertex::operator+(const Vertex & val) const
{
	Vertex result;
	result.position = position + val.position;
	result.position.w = position.w + val.position.w;
	result.rhw = rhw + val.rhw;
	result.texCoord.u = texCoord.u + val.texCoord.u;
	result.texCoord.v = texCoord.v + val.texCoord.v;
	result.color.red = color.red + val.color.red;
	result.color.green = color.green + val.color.green;
	result.color.blue = color.blue + val.color.blue;
	return result;
}

void Vertex::operator+=(const Vertex & val)
{
	position += val.position;
	position.w += val.position.w;
	rhw += val.rhw;
	texCoord.u += val.texCoord.u;
	texCoord.v += val.texCoord.v;
	color.red += val.color.red;
	color.green += val.color.green;
	color.blue += val.color.blue;
}

Vertex Vertex::operator-(const Vertex & val) const
{
	Vertex result;
	result.position.x	= position.x - val.position.x;
	result.position.y	= position.y - val.position.y;
	result.position.z	= position.z - val.position.z;
	result.position.w	= position.w - val.position.w;
	result.rhw			= rhw - val.rhw;	
	result.texCoord.u	= texCoord.u - val.texCoord.u;
	result.texCoord.v	= texCoord.v - val.texCoord.v;
	result.color.red	= color.red - val.color.red;
	result.color.green	= color.green - val.color.green;
	result.color.blue	= color.blue - val.color.blue;
	return result;
}

Vertex Vertex::Division(const Vertex & vertex, float w) const
{
	Vertex result;

	float inv = 1.0f / w;

	result.position.x = (vertex.position.x - position.x) * inv;
	result.position.y = (vertex.position.y - position.y) * inv;
	result.position.z = (vertex.position.z - position.z) * inv;
	result.position.w = (vertex.position.w - position.w) * inv;
	result.texCoord.u = (vertex.texCoord.u - texCoord.u) * inv;
	result.texCoord.v = (vertex.texCoord.v - texCoord.v) * inv;
	result.color.red = (vertex.color.red - color.red) * inv;
	result.color.green = (vertex.color.green - color.green) * inv;
	result.color.blue = (vertex.color.blue - color.blue) * inv;
	result.rhw = (vertex.rhw - rhw) * inv;
	return result;
}

void Vertex::RHWInit()
{
	float _rhw = 1.0f / position.w;
	rhw = _rhw;
	texCoord.u *= rhw;
	texCoord.v *= rhw;
	color.red *= rhw;
	color.green *= rhw;
	color.blue *= rhw;
}

void Vertex::Lerp(const Vertex & a, const Vertex & b, float t)
{
	position.Lerp(a.position, b.position, t);
	texCoord.u = lerp(a.texCoord.u, b.texCoord.u, t);
	texCoord.v = lerp(a.texCoord.v, b.texCoord.v, t);
	color.red = lerp(a.color.red, b.color.red, t);
	color.green = lerp(a.color.green, b.color.green, t);
	color.blue = lerp(a.color.blue, b.color.blue, t);
	rhw = lerp(a.rhw, b.rhw, t);
}