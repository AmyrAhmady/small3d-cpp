#include "World.hpp"
#include <iostream>

World::World()
{}

World::~World()
{}

World::World(int width, int height, void * frameBuff)
{
	int need = sizeof(void *) * (height * 2 + 1024) + width * height * 8;
	char * ptr = (char *)malloc(need + 64);
	char * fb, * zbuf;

	frameBuffer = reinterpret_cast<DWORD **>(ptr);
	zBuffer = reinterpret_cast<float **>(ptr + sizeof(void *) * height);
	ptr += sizeof(void *) * height * 2;
	texture = reinterpret_cast<DWORD **>(ptr);
	ptr += sizeof(void *) * 1024;
	fb = static_cast<char *>(ptr);
	zbuf = static_cast<char *>(ptr + width * height * 4);
	ptr += width * height * 8;
	if (frameBuff != NULL) fb = (char *)frameBuff;
	for (int i = 0; i < height; i++)
	{
		frameBuffer[i] = reinterpret_cast<DWORD *>(fb + width * 4 * i);
		zBuffer[i] = (float *)(zbuf + width * 4 * i);
	}
	texture[0] = reinterpret_cast<DWORD *>(ptr);
	texture[1] = reinterpret_cast<DWORD *>(ptr + 16);
	memset(texture[0], 0, 64);
	texWidth = 2;
	texHeight = 2;
	maxu = 1.0f;
	maxv = 1.0f;
	this->width = width;
	this->height = height;
	background = 0xc0c0c0;
	foreground = 0;
	transform = Transform(static_cast<float>(height), static_cast<float>(width));

	renderType = RENDER_TYPE::TEXTURE;
}

void World::Clear(int mode)
{
	int y, x, h = height;
	for (y = 0; y < height; y++)
	{
		DWORD * dst = frameBuffer[y];
		DWORD cc = (h - 1 - y) * 230 / (h - 1);
		cc = (cc << 16) | (cc << 8) | cc;
		if (mode == 0) cc = background;
		for (x = width; x > 0; dst++, x--) dst[0] = cc;
	}
	for (y = 0; y < height; y++)
	{
		float * dst = zBuffer[y];
		for (x = width; x > 0; dst++, x--) dst[0] = 0.0f;
	}
}

void World::SetTexture(void * bits, long pitch, int w, int h)
{
	char * ptr = (char *)bits;
	for (int i = 0; i < h; ptr += pitch, i++)
	{
		texture[i] = reinterpret_cast<DWORD *>(ptr);
	}
	texWidth = w;
	texHeight = h;
	maxu = static_cast<float>(w - 1);
	maxv = static_cast<float>(h - 1);
}

DWORD World::GetTextureColor(float u, float v)
{
	int x, y;
	u = u * maxu;
	v = v * maxv;
	x = static_cast<int>(u + 0.5f);
	y = static_cast<int>(v + 0.5f);
	x = clamp(x, 0, texWidth - 1);
	y = clamp(y, 0, texHeight - 1);
	return texture[y][x];
}

void World::ApplyMesh(const Vertex * mesh)
{
	modelMesh = const_cast<Vertex *>(mesh);
}

void World::SetPixel(Point<int> point, DWORD color)
{
	if (static_cast<DWORD>(point.x) < static_cast<DWORD>(width) && static_cast<DWORD>(point.y) < static_cast<DWORD>(height))
	{
		frameBuffer[point.x][point.y] = color;
	}
}

void World::SetPixel(int x, int y, DWORD color)
{
	SetPixel({ x, y }, color);
}

void World::DrawLine(Point<int> point1, Point<int> point2, DWORD color)
{
	int x, y, rem = 0;
	if (point1.x == point2.x && point1.y == point2.y)
	{
		SetPixel(point1.x, point1.y, color);
	}
	else if (point1.x == point2.x)
	{
		int inc = (point1.y <= point2.y) ? 1 : -1;
		for (y = point1.y; y != point2.y; y += inc) SetPixel(point1.x, y, color);
		SetPixel(point2.x, point2.y, color);
	}
	else if (point1.y == point2.y)
	{
		int inc = (point1.x <= point2.x) ? 1 : -1;
		for (x = point1.x; x != point2.x; x += inc) SetPixel(x, point1.y, color);
		SetPixel(point2.x, point2.y, color);
	}
	else
	{
		int dx = (point1.x < point2.x) ? point2.x - point1.x : point1.x - point2.x;
		int dy = (point1.y < point2.y) ? point2.y - point1.y : point1.y - point2.y;
		if (dx >= dy)
		{
			if (point2.x < point1.x)
			{
				Point<int> temp;
				temp = point1;
				point1 = point2;
				point2 = temp;
				x = temp.x, y = temp.y;
			}
			for (x = point1.x, y = point1.y; x <= point2.x; x++)
			{
				SetPixel(x, y, color);
				rem += dy;
				if (rem >= dx)
				{
					rem -= dx;
					y += (point2.y >= point1.y) ? 1 : -1;
					SetPixel(x, y, color);
				}
			}
			SetPixel(point2.x, point2.y, color);
		}
		else
		{
			if (point2.y < point1.y)
			{
				Point<int> temp;
				temp = point1;
				point1 = point2;
				point2 = temp;
				x = temp.x, y = temp.y;
			}
			for (x = point1.x, y = point1.y; y <= point2.y; y++)
			{
				SetPixel(x, y, color);
				rem += dx;
				if (rem >= dy)
				{
					rem -= dy;
					x += (point2.x >= point1.x) ? 1 : -1;
					SetPixel(x, y, color);
				}
			}
			SetPixel(point2.x, point2.y, color);
		}
	}
}

void World::DrawScanline(Scanline & scanline)
{
	DWORD * framebuffer = frameBuffer[scanline.y];
	float * zbuffer = zBuffer[scanline.y];
	int x = scanline.x;
	int w = scanline.w;

	for (; w > 0; x++, w--)
	{
		if (x >= 0 && x < width)
		{
			float rhw = scanline.vertex.rhw;
			if (rhw >= zbuffer[x])
			{
				float w = 1.0f / rhw;
				zbuffer[x] = rhw;
				if (renderType == RENDER_TYPE::TEXTURE)
				{
					float u = scanline.vertex.texCoord.u * w;
					float v = scanline.vertex.texCoord.v * w;
					DWORD cc = GetTextureColor(u, v);
					framebuffer[x] = cc;
				}
			}
		}
		scanline.vertex += scanline.step;
		if (x >= width) break;
	}
}

void World::DrawTrapezoid(Trapezoid & trapezoid)
{
	Scanline scanline;
	int top = static_cast<int>(trapezoid.top + 0.5f);
	int bottom = static_cast<int>(trapezoid.bottom + 0.5f);

	for (int i = top; i < bottom; i++)
	{
		if (i >= 0 && i < height)
		{
			trapezoid.EdgeLerp(static_cast<float>(i) + 0.5f);
			trapezoid.ScanlineInit(scanline, i);
			DrawScanline(scanline);
		}
		if (i >= height) break;
	}
}

void World::DrawPrimitive(const Vertex & v1, const Vertex & v2, const Vertex & v3)
{
	Vector
		point1,
		point2,
		point3,
		c1,
		c2,
		c3;

	c1 = transform.Apply(v1.position);
	c2 = transform.Apply(v2.position);
	c3 = transform.Apply(v3.position);

	if (CheckCVV(c1) != 0) return;
	if (CheckCVV(c2) != 0) return;
	if (CheckCVV(c3) != 0) return;

	point1 = transform.Homogenize(c1);
	point2 = transform.Homogenize(c2);
	point3 = transform.Homogenize(c3);

	if (renderType == RENDER_TYPE::TEXTURE)
	{
		Vertex
			t1 = v1,
			t2 = v2,
			t3 = v3;

		Trapezoid traps[2];

		t1.position = point1;
		t2.position = point2;
		t3.position = point3;
		t1.position.w = c1.w;
		t2.position.w = c2.w;
		t3.position.w = c3.w;

		t1.RHWInit();
		t2.RHWInit();
		t3.RHWInit();

		int number = Trapezoid::TriangleInit(traps, t1, t2, t3);

		if (number >= 1) DrawTrapezoid(traps[0]);
		if (number >= 2) DrawTrapezoid(traps[1]);
	}
}

void World::DrawPlane(int a, int b, int c, int d)
{
	Vertex
		p1 = modelMesh[a],
		p2 = modelMesh[b],
		p3 = modelMesh[c],
		p4 = modelMesh[d];

	p1.texCoord.u = 0;
	p1.texCoord.v = 0;

	p2.texCoord.u = 0;
	p2.texCoord.v = 1;

	p3.texCoord.u = 1;
	p3.texCoord.v = 1;

	p4.texCoord.u = 1;
	p4.texCoord.v = 0;

	DrawPrimitive(p1, p2, p3);
	DrawPrimitive(p3, p4, p1);
}

void World::DrawBox(float theta)
{
	Matrix m;
	m.SetRotate(-1, -0.5, 1, theta);
	transform.world = m;
	transform.Update();
	DrawPlane(0, 1, 2, 3);
	DrawPlane(7, 6, 5, 4);
	DrawPlane(0, 4, 5, 1);
	DrawPlane(1, 5, 6, 2);
	DrawPlane(2, 6, 7, 3);
	DrawPlane(3, 7, 4, 0);
}

void World::SetEyeData(const Vector & position, const Vector & lookAt)
{
	Vector up = { 0, 0, 1, 1 };
	transform.view.SetLookAt(position, lookAt, up);
	transform.Update();
}