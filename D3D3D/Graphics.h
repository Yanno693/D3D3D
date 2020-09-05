#pragma once
#include "Includes.h"
#include "D3D3DException.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>

#define GFX_THROW_FAILED(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::Exception( __LINE__,__FILE__,hr )
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )

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
	
	Graphics(HWND _hWnd, int _w, int _h);
	Graphics(const Graphics& g) = delete;
	Graphics& operator=(const Graphics& g) = delete;
	//~Graphics() = default;
	void endFrame();
	void clearBuffer(float _r, float _g, float _b); // ClearBuffer color

	void drawTestTriangle(float time, float* cameraPosition, float* cameraRotation);

	// ComPtr automatically release the resource, because COM = has to be released
	// a bit like smart pointers
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget;
	int w, h;

	// Raytracing utilities

	// 1. Inverse matrix transformation
	struct C_Shared { // Shared informations for the whole pixel shader
		struct {
			DirectX::XMMATRIX t;
		} transformation;
	};

	struct Vertex {
		float x, y;
	};
	
	C_Shared shared[1];

	const Vertex vertices[4] = {
		{ -1, 1},
		{ 1, 1},
		{ -1, -1},
		{ 1, -1},
	};

	DirectX::XMMATRIX inverseTransformMatrix; // Trnsform for raycasting

	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer_ptr; // Pass of constantes and matrices
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer_ptr; // Pass of the 4 vertices for rendering

	D3D11_BUFFER_DESC consantDesc;
	D3D11_SUBRESOURCE_DATA constantSubResourceData;

	// Input layer : link between our Vertex structure and the shader ... i guess ?
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayer_ptr;
	const D3D11_INPUT_ELEMENT_DESC inputLayerDesc[1] =
	{
		{
			"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader_ptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader_ptr;


};