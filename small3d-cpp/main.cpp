#include <iostream>
#include <cstdlib>
#include <ctime>
#include "World/World.hpp"
#include "Window/Window.hpp"

#include "Textures/Bricks.hpp"
#include "Textures/Obama.hpp"

DWORD texture[256][256];

Vertex mesh[8] = {
	//{ Vector pos }, { Color color }, { TextCoord tc }, float rhw
	{ { -1, -1,  1, 1 }, { 1.0f, 0.2f, 0.2f }, { 0, 0 }, 1.0f },
	{ {  1, -1,  1, 1 }, { 0.2f, 1.0f, 0.2f }, { 0, 1 }, 1.0f },
	{ {  1,  1,  1, 1 }, { 0.2f, 0.2f, 1.0f }, { 1, 1 }, 1.0f },
	{ { -1,  1,  1, 1 }, { 1.0f, 0.2f, 1.0f }, { 1, 0 }, 1.0f },
	{ { -1, -1, -1, 1 }, { 1.0f, 1.0f, 0.2f }, { 0, 0 }, 1.0f },
	{ {  1, -1, -1, 1 }, { 0.2f, 1.0f, 1.0f }, { 0, 1 }, 1.0f },
	{ {  1,  1, -1, 1 }, { 1.0f, 0.3f, 0.3f }, { 1, 1 }, 1.0f },
	{ { -1,  1, -1, 1 }, { 0.2f, 1.0f, 0.3f }, { 1, 0 }, 1.0f }
};

void GenerateDummyTexture(DWORD texture[][256])
{
	std::srand(time(0));

	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			texture[i][j] = (rand() % 20 + 1) / 20 ? 0xe6ffff : 0x110000;
		}
	}
}

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	int indicator = 1;
	int holdingSpace = 0;
	float rotation = 1;
	float posX = 3.5;

	const TCHAR * title = L"small3d-cpp";

	Window window = Window(800, 600, title);

	World world = World(800, 600, Window::windowFrameBuffer);
	world.SetEyeData({ 3.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f , 1.0f });

	world.SetTexture(bricksTexture, 256 * 4, 256, 256);

	world.ApplyMesh(mesh);

	while (Window::exitCode == 0 && Window::keys[VK_ESCAPE] == 0)
	{
		window.Dispatch();
		world.Clear(1);
		world.SetEyeData({ posX, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });

		if (Window::keys[VK_UP]) posX -= 0.01f;
		if (Window::keys[VK_DOWN]) posX += 0.01f;
		if (Window::keys[VK_LEFT]) rotation += 0.01f;
		if (Window::keys[VK_RIGHT]) rotation -= 0.01f;

		if (Window::keys[VK_SPACE])
		{
			if (holdingSpace == 0)
			{
				holdingSpace = 1;
				if (++indicator >= 3) indicator = 0;

				if (!indicator)
				{
					GenerateDummyTexture(texture);
					world.SetTexture(reinterpret_cast<unsigned char *>(texture), 256 * 4, 256, 256);
				}
				else if (indicator == 1)
				{
					world.SetTexture(bricksTexture, 256 * 4, 256, 256);
				}
				else if (indicator == 2)
				{
					world.SetTexture(obamaTexture, 256 * 4, 256, 256);
				}

			}
		}
		else
		{
			holdingSpace = 0;
		}

		world.DrawBox(rotation);
		window.Update();
		Sleep(1);
	}
}

