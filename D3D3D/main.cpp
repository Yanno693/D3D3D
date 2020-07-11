#include <Windows.h>
#include <string>
#include <sstream>

LRESULT CALLBACK winloop(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	
	//OutputDebugString((msg).c_str());
	//LPWSTR str = L"ok";
	//wsprintf(str, L"%d", msg);
	//LPCWSTR str = L"lol";
	//LPWSTR str = "lol";
	//OutputDebugString(str);
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
			//OutputDebugString(mstr);
			OutputDebugString(buf);
			
			delete[] buf;
			break;
		}
		default:
			break;
	}
	
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int nCmdShow )
{
	// 1. Register window

	LPCWSTR className = L"D3D3D"; // ClassName
	
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = winloop; // Window event loop
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hIconSm = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = className;
	RegisterClassEx(&wc);
	
	// 2. Instantiate window
	HWND hWnd = CreateWindowEx(
		0,
		className,
		L"Fenetreux",
		WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		200, 200,
		640, 480,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	// 3.
	ShowWindow(hWnd, SW_SHOW);

	MSG message;
	BOOL res;

	while ((res = GetMessage(&message, nullptr, 0, 0)) > 0) {
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}

	if (res == -1) {
		return -1;
	}
	else
	{
		return message.wParam;
	}

	return 0;
}