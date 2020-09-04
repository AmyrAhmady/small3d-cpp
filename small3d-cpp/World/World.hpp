#pragma once

#include <Windows.h>

#include "../Transform/Transform.hpp"
#include "../Trapezoid/Trapezoid.hpp"
#include "../Vertex/Vertex.hpp"
#include "../common.hpp"

class World
{
public:
	Transform transform;
	int width;
	int height;
	Vertex * modelMesh;
	DWORD ** frameBuffer;
	float ** zBuffer;
	DWORD ** texture;
	int texWidth;
	int texHeight;
	float maxu;
	float maxv;
	RENDER_TYPE renderType;
	DWORD background;
	DWORD foreground;

	World();
	~World();

	World(int width, int height, void * frameBuff);

	void Clear(int mode);

	void SetTexture(void * bits, long pitch, int w, int h);
	DWORD GetTextureColor(float u, float v);

	void ApplyMesh(const Vertex * mesh);

	void SetPixel(Point<int> point, DWORD color);
	void SetPixel(int x, int y, DWORD color);
	void DrawLine(Point<int> point1, Point<int> point2, DWORD color);

	void DrawScanline(Scanline & scanline);
	void DrawTrapezoid(Trapezoid & trapezoid);
	void DrawPrimitive(const Vertex & v1, const Vertex & v2, const Vertex & v3);
	void DrawPlane(int a, int b, int c, int d);

	void DrawBox(float theta);

	void SetEyeData(const Vector & position, const Vector & lookAt);

	static int CheckCVV(const Vector & vec) 
	{
		int check = 0;
		if (vec.z < 0.0f) check |= 1;
		if (vec.z > vec.w) check |= 2;
		if (vec.x < -vec.w) check |= 4;
		if (vec.x > vec.w) check |= 8;
		if (vec.y < -vec.w) check |= 16;
		if (vec.y > vec.w) check |= 32;
		return check;
	}
};