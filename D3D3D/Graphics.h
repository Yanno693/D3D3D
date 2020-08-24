#pragma once
#include "Includes.h"
#include "D3D3DException.h"
#include <d3d11.h>
#include <wrl.h>

class Graphics {
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

	class DeviceRemovedException : public Exception {
	public:
		DeviceRemovedException(int _line, const char* _file, HRESULT _hResult);
		virtual const char* getType() const noexcept;
	};
	
	Graphics(HWND _hWnd);
	Graphics(const Graphics& g) = delete;
	Graphics& operator=(const Graphics& g) = delete;
	//~Graphics() = default;
	void endFrame();
	void clearBuffer(float _r, float _g, float _b); // ClearBuffer color

	//ID3D11DeviceContext* deviceContext;
	//ID3D11Device* device;
	//IDXGISwapChain* swapChain;
	//ID3D11RenderTargetView* renderTarget;

	// ComPtr automatically release the resource, because COM = has to be released
	// a bit like smart pointers
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget;

};