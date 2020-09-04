#pragma once

#include "Vertex/Vertex.hpp"

enum class RENDER_TYPE
{
	TEXTURE
};

struct Scanline
{
	Vertex vertex;
	Vertex step;
	int x;
	int y;
	int w;
};

struct Edge
{
	Vertex vertex;
	Vertex vertex1;
	Vertex vertex2;
};

template<typename T>
struct Point
{
	T x;
	T y;
};
