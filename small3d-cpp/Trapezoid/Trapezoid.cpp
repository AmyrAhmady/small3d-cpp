#include "Trapezoid.hpp"
#include <iostream>

Trapezoid::Trapezoid()
{}

Trapezoid::~Trapezoid()
{}

int Trapezoid::TriangleInit(Trapezoid * trapezoids, Vertex & a, Vertex & b, Vertex & c)
{
	Vertex tempVertex;
	float k, x;

	if (a.position.y > b.position.y)
	{
		tempVertex = a;
		a = b;
		b = tempVertex;
	}

	if (a.position.y > c.position.y)
	{
		tempVertex = a;
		a = c;
		c = tempVertex;
	}

	if (b.position.y > c.position.y)
	{
		tempVertex = b;
		b = c;
		c = tempVertex;
	}

	if (a.position.y == b.position.y && a.position.y == c.position.y)
	{
		return 0;
	}

	if (a.position.x == b.position.x && a.position.x == c.position.x)
	{
		return 0;
	}

	if (a.position.y == b.position.y) 	// triangle down
	{
		if (a.position.x > b.position.x)
		{
			tempVertex = a;
			a = b;
			b = tempVertex;
		}

		trapezoids[0].top = a.position.y;
		trapezoids[0].bottom = c.position.y;
		trapezoids[0].left.vertex1 = a;
		trapezoids[0].left.vertex2 = c;
		trapezoids[0].right.vertex1 = b;
		trapezoids[0].right.vertex2 = c;
		return (trapezoids[0].top < trapezoids[0].bottom) ? 1 : 0;
	}

	if (b.position.y == c.position.y) 	// triangle up
	{
		if (b.position.x > c.position.x)
		{
			tempVertex = b;
			b = c;
			c = tempVertex;
		}

		trapezoids[0].top = a.position.y;
		trapezoids[0].bottom = c.position.y;
		trapezoids[0].left.vertex1 = a;
		trapezoids[0].left.vertex2 = b;
		trapezoids[0].right.vertex1 = a;
		trapezoids[0].right.vertex2 = c;
		return (trapezoids[0].top < trapezoids[0].bottom) ? 1 : 0;
	}

	trapezoids[0].top = a.position.y;
	trapezoids[0].bottom = b.position.y;
	trapezoids[1].top = b.position.y;
	trapezoids[1].bottom = c.position.y;

	k = (c.position.y - a.position.y) / (b.position.y - a.position.y);
	x = a.position.x + (b.position.x - a.position.x) * k;

	if (x <= c.position.x)      // triangle left
	{
		trapezoids[0].left.vertex1 = a;
		trapezoids[0].left.vertex2 = b;
		trapezoids[0].right.vertex1 = a;
		trapezoids[0].right.vertex2 = c;
		trapezoids[1].left.vertex1 = b;
		trapezoids[1].left.vertex2 = c;
		trapezoids[1].right.vertex1 = a;
		trapezoids[1].right.vertex2 = c;
	}
	else                       // triangle right
	{
		trapezoids[0].left.vertex1 = a;
		trapezoids[0].left.vertex2 = c;
		trapezoids[0].right.vertex1 = a;
		trapezoids[0].right.vertex2 = b;
		trapezoids[1].left.vertex1 = a;
		trapezoids[1].left.vertex2 = c;
		trapezoids[1].right.vertex1 = b;
		trapezoids[1].right.vertex2 = c;
	}

	return 2;
}

void Trapezoid::ScanlineInit(Scanline & scanline, int y)
{
	
	float width = right.vertex.position.x - left.vertex.position.x;

	scanline.x = static_cast<int>(left.vertex.position.x + 0.5f);
	scanline.w = static_cast<int>(right.vertex.position.x + 0.5f) - scanline.x;
	scanline.y = y;
	scanline.vertex = left.vertex;

	if (left.vertex.position.x >= right.vertex.position.x)
	{
		scanline.w = 0;
	}

	scanline.step = left.vertex.Division(right.vertex, width);
}

void Trapezoid::EdgeLerp(float t)
{
	float s1 = left.vertex2.position.y - left.vertex1.position.y;
	float s2 = right.vertex2.position.y - right.vertex1.position.y;
	float t1 = (t - left.vertex1.position.y) / s1;
	float t2 = (t - right.vertex1.position.y) / s2;
	left.vertex.Lerp(left.vertex1, left.vertex2, t1);
	right.vertex.Lerp(right.vertex1, right.vertex2, t2);
}