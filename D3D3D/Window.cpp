#include <Windows.h>
//#include "Includes.h" ?
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

Window::Exception::Exception(int _line, const char* _file, HRESULT _hResult) : D3D3DException(_line, _file), hResult(_hResult) {}

const char *Window::Exception::what() const noexcept {
	std::ostringstream stream;
	stream << getType() << std::endl << " [Error Code] " << getErrorCode() << std::endl << " [Description] " << getErrorString() << std::endl << getOriginString();
	whatBuffer = stream.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::getType() const noexcept {
	return "D3D3D Window Exception";
}

std::string Window::Exception::translateErrorCode(HRESULT _hResult) noexcept {

	LPTSTR pMsgBuf;
	// windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		_hResult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&pMsgBuf, 0, nullptr
	);

	//FormatMessage()
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::wstring wst(pMsgBuf);
	std::string errorString(wst.begin(), wst.end());
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;

}

HRESULT Window::Exception::getErrorCode() const noexcept {
	return hResult;
}

std::string Window::Exception::getErrorString() const noexcept {
	return translateErrorCode(hResult);
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
		return pWnd->handleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::winLoop(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->handleMsg(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {

	wchar_t mstr[2];
	mstr[1] = '\0';

	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(69);
		return 0;
	case WM_KILLFOCUS: // When focus of the window is lost, clear keystate
		kb.ClearState();
		break;
	// KEYBOARD MESSAGE
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kb.AutorepeatIsEnabled()) {
			kb.OnKeyPressed(static_cast<unsigned char>(wParam));
		}

		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kb.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kb.OnChar(static_cast<unsigned char>(wParam));
		break;
	// END KEYBOARD MESSAGE
	}

	//switch (msg) {
	//case WM_CLOSE:
	//	PostQuitMessage(69);
	//	break;
	//case WM_KEYDOWN:
	//	mstr[0] = LOWORD(wParam); // Get Char
	//	OutputDebugString(L"Key Pressed : ");
	//	OutputDebugString(mstr);
	//	OutputDebugString(L"\n");
	//	break;
	//case WM_LBUTTONDOWN: {
	//	POINTS pts = MAKEPOINTS(lParam);
	//	int xPos = pts.x;
	//	int yPos = pts.y;
	//	std::ostringstream ss;
	//	ss << xPos << " " << yPos << std::endl;

	//	wchar_t* buf = new wchar_t[4096];
	//	MultiByteToWideChar(CP_ACP, 0, ss.str().c_str(), -1, buf, 4096);

	//	OutputDebugString(L"Mouse: ");
	//	OutputDebugString(buf);

	//	delete[] buf;
	//	break;
	//}
	//case WM_MOUSEMOVE: {
	//	POINTS pts = MAKEPOINTS(lParam);
	//	int xPos = pts.x;
	//	int yPos = pts.y;
	//	std::ostringstream ss;
	//	ss << xPos << " " << yPos << std::endl;

	//	wchar_t* buf = new wchar_t[4096];
	//	MultiByteToWideChar(CP_ACP, 0, ss.str().c_str(), -1, buf, 4096);

	//	OutputDebugString(L"Mouse: ");
	//	OutputDebugString(buf);

	//	delete[] buf;
	//	break;
	//}
	//default:
	//	break;
	//}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Window(int _w, int _h, const LPCWSTR _name) {
	w = _w;
	h = _h;

	RECT rect;
	rect.left = 200;
	rect.right = 200 + w;
	rect.top = 200;
	rect.bottom = 200 + h;
	AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

	HWND hWnd = CreateWindowEx(
		0,
		WindowClass::getName(),
		_name,
		WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		nullptr,
		nullptr,
		WindowClass::getInstance(),
		this);

	if (hWnd == nullptr) {
		throw D3D3D_LAST();
	}

	//throw D3D3D_EXCEPT(7);

	ShowWindow(hWnd, SW_SHOW);
}

Window::~Window() {
	DestroyWindow(hWnd);
}