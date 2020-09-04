#include "Window.hpp"

int Window::exitCode = 0;
unsigned char * Window::windowFrameBuffer = NULL;
int Window::keys[512];

Window::Window()
{}

Window::~Window()
{
	if (DC)
	{
		if (objectBitmap)
		{
			SelectObject(DC, objectBitmap);
			objectBitmap = NULL;
		}
		DeleteDC(DC);
		DC = NULL;
	}
	if (handleBitmap)
	{
		DeleteObject(handleBitmap);
		handleBitmap = NULL;
	}
	if (handle)
	{
		CloseWindow(handle);
		handle = NULL;
	}
}

Window::Window(int w, int h, const TCHAR * title)
{
	WNDCLASS windowClass = {
		CS_BYTEALIGNCLIENT,
		reinterpret_cast<WNDPROC>(WndProc),
		0,
		0,
		0,
		NULL,
		NULL,
		NULL,
		NULL,
		L"SCREEN3.1415926"
	};

	BITMAPINFO bi = {
		{
			sizeof(BITMAPINFOHEADER),
			w,
			-h,
			1,
			32,
			BI_RGB,
			w * h * 4,
			0,
			0,
			0,
			0
		}
	};

	RECT rect = { 0, 0, w, h };
	int wx, wy, sx, sy;
	LPVOID ptr;
	HDC hDC;

	this->~Window();

	windowClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClass(&windowClass);

	handle = CreateWindow(L"SCREEN3.1415926", title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, 0, 0, NULL, NULL, windowClass.hInstance, NULL);

	exitCode = 0;
	hDC = GetDC(handle);
	DC = CreateCompatibleDC(hDC);
	ReleaseDC(handle, hDC);

	handleBitmap = CreateDIBSection(DC, &bi, DIB_RGB_COLORS, &ptr, 0, 0);

	objectBitmap = static_cast<HBITMAP>(SelectObject(DC, handleBitmap));
	windowFrameBuffer = static_cast<unsigned char *>(ptr);
	width = w;
	height = h;
	pitch = w * 4;

	AdjustWindowRect(&rect, GetWindowLong(handle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(handle);

	ShowWindow(handle, SW_NORMAL);
	Dispatch();
}


LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE: 
		exitCode = 1; 
		break;
	case WM_KEYDOWN: 
		keys[wParam & 511] = 1; 
		break;
	case WM_KEYUP: 
		keys[wParam & 511] = 0; 
		break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void Window::Dispatch(void)
{
	MSG msg;
	while (1) 
	{
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void Window::Update(void) {
	HDC hDC = GetDC(handle);
	BitBlt(hDC, 0, 0, width, height, DC, 0, 0, SRCCOPY);
	ReleaseDC(handle, hDC);
	Dispatch();
}