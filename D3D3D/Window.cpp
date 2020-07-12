#include <Windows.h>
#include "Window.h"

Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass() noexcept {
	hInstance = GetModuleHandle(nullptr);
	
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	//wc.lpfnWndProc = winloop; // Window event loop
	wc.lpfnWndProc = winInit;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = getInstance();
	wc.hIcon = nullptr;
	wc.hIconSm = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = getName();
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
	UnregisterClass(wndClassName, getInstance());
}

LPCWSTR Window::WindowClass::getName() noexcept {
	return wndClassName;
}

HINSTANCE Window::WindowClass::getInstance() noexcept {
	return windowClass.hInstance;
}

LRESULT WINAPI Window::winInit(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// store window class pointer to WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::winLoop));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::winLoop(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {

	wchar_t mstr[2];
	mstr[1] = '\0';

	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	case WM_KEYDOWN:
		mstr[0] = LOWORD(wParam); // Get Char
		OutputDebugString(L"Key Pressed : ");
		OutputDebugString(mstr);
		OutputDebugString(L"\n");
		break;
	case WM_LBUTTONDOWN: {
		POINTS pts = MAKEPOINTS(lParam);
		int xPos = pts.x;
		int yPos = pts.y;
		std::ostringstream ss;
		ss << xPos << " " << yPos << std::endl;

		wchar_t* buf = new wchar_t[4096];
		MultiByteToWideChar(CP_ACP, 0, ss.str().c_str(), -1, buf, 4096);

		OutputDebugString(L"Mouse: ");
		OutputDebugString(buf);

		delete[] buf;
		break;
	}
	case WM_MOUSEMOVE: {
		POINTS pts = MAKEPOINTS(lParam);
		int xPos = pts.x;
		int yPos = pts.y;
		std::ostringstream ss;
		ss << xPos << " " << yPos << std::endl;

		wchar_t* buf = new wchar_t[4096];
		MultiByteToWideChar(CP_ACP, 0, ss.str().c_str(), -1, buf, 4096);

		OutputDebugString(L"Mouse: ");
		OutputDebugString(buf);

		delete[] buf;
		break;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Window(int _w, int _h, const LPCWSTR _name) noexcept {
	w = _w;
	h = _h;
	//name = _name;

	HWND hWnd = CreateWindowEx(
		0,
		WindowClass::getName(),
		//_name,
		L"KAKA",
		WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		200, 200,
		640, 480,
		nullptr,
		nullptr,
		WindowClass::getInstance(),
		this);

	ShowWindow(hWnd, SW_SHOW);
}

Window::~Window() {
	if (name)
		delete[] name;
	DestroyWindow(hWnd);
}