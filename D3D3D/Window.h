#pragma once
#include "Includes.h"

class Window {
private:
	class WindowClass {
	public:
		//const char* getName();
		static LPCWSTR getName() noexcept;
		static HINSTANCE getInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		//const char* wndClassName = "D3D3D";
		static constexpr LPCWSTR wndClassName = L"D3D3D";
		static WindowClass windowClass; // Only one instance of window class
		HINSTANCE hInstance;
	};

public:
	Window(int _w, int _h, const LPCWSTR _name) noexcept; // No except : Check at compil time if it can throw exeptions
	~Window();

	int w, h;
	char* name;
	HWND hWnd;

	static LRESULT WINAPI winInit(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI winLoop(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};