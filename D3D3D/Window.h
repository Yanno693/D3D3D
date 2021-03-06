#pragma once
#include "Includes.h"
#include "D3D3DException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <optional>
#include <memory>

class Window {
private:
	
	class WindowClass {
	public:
		static LPCWSTR getName();
		static HINSTANCE getInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr LPCWSTR wndClassName = L"D3D3D";
		static WindowClass windowClass; // Only one instance of window class
		HINSTANCE hInstance;
	};

	int w, h;
	HWND hWnd;

public:

	class Exception : public D3D3DException {
	public:
		Exception(int _line, const char* _file, HRESULT _hResult);
		const char* what() const noexcept override;
		virtual const char* getType() const noexcept;
		static std::string translateErrorCode(HRESULT _hResult) noexcept;
		HRESULT getErrorCode() const noexcept;
		std::string getErrorString() const noexcept;
	private:
		HRESULT hResult;
	};

	Window(int _w, int _h, const LPCWSTR _name); // No except : Check at compil time if it can throw exeptions
	~Window();

	static LRESULT WINAPI winInit(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI winLoop(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	void setTitle(const LPCWSTR& _name);
	static std::optional<int> processMessages();
	Graphics& gfx();

	Keyboard kb;
	Mouse mouse;
	//Graphics gfx;
	std::unique_ptr<Graphics> gfx_ptr;
};

#define D3D3D_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define D3D3D_LAST() Window::Exception(__LINE__, __FILE__, GetLastError())