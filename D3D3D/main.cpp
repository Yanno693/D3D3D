#include <Windows.h>
#include "Window.h"

LRESULT CALLBACK winloopp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

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

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int nCmdShow )
{
	try {
		Window w = Window(640, 480, L"Salut");

		MSG message;
		BOOL res;

		while ((res = GetMessage(&message, nullptr, 0, 0)) > 0) {
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}

		if (res == -1)
			return -1;
		return message.wParam;
	}

	catch (const D3D3DException& e) {
		MessageBeep(MB_OK);

		wchar_t* buf1 = new wchar_t[4096];
		wchar_t* buf2 = new wchar_t[4096];
		MultiByteToWideChar(CP_ACP, 0, e.what(), -1, buf1, 4096);
		MultiByteToWideChar(CP_ACP, 0, e.getType(), -1, buf2, 4096);
		MessageBox(nullptr, buf1, buf2, MB_OK);
	}
	catch (const std::exception& e) {
		MessageBox(nullptr, L"No details", L"Unknown exception", MB_OK);
	}

	return 0;
}