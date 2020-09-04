#pragma once

#include "../Vertex/Vertex.hpp"
#include "../common.hpp"

class Trapezoid
{
public:
	float top;
	float bottom;
	Edge left;
	Edge right;

	Trapezoid();
	~Trapezoid();

	static int TriangleInit(Trapezoid * trapezoids, Vertex & a, Vertex & b, Vertex & c);

	void ScanlineInit(Scanline & scanline, int y);
	void EdgeLerp(float t);

};