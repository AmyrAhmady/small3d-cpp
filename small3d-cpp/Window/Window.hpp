#pragma once

#include <Windows.h>

class Window
{
public:
	int width;
	int height;
	long pitch = 0;
	static int exitCode;
	static int keys[512];
	HWND handle = NULL;
	HDC DC = NULL;
	HBITMAP handleBitmap = NULL;
	HBITMAP objectBitmap = NULL;
	static unsigned char * windowFrameBuffer;

	Window();
	~Window();

	Window(int w, int h, const TCHAR * title);
	
	void Dispatch();
	void Update();

	static LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
};