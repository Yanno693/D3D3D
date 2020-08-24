#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")
// Basically some dark magic

#define GFX_THROW_FAILED(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::Exception( __LINE__,__FILE__,hr )
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )

Graphics::Exception::Exception(int _line, const char* _file, HRESULT _hResult) : D3D3DException(_line, _file), hResult(_hResult) {}

const char* Graphics::Exception::what() const noexcept {
	std::ostringstream stream;
	stream << getType() << std::endl << " [Error Code] " << getErrorCode() << std::endl << " [Description] " << getErrorString() << std::endl << getOriginString();
	whatBuffer = stream.str();
	return whatBuffer.c_str();
}

const char* Graphics::Exception::getType() const noexcept {
	return "D3D3D Graphics Exception";
}

Graphics::DeviceRemovedException::DeviceRemovedException(int _line, const char* _file, HRESULT _hResult) : Exception(_line, _file, _hResult) {}

const char* Graphics::DeviceRemovedException::getType() const noexcept {
	return "D3D3D Device Removed Exception";
}

std::string Graphics::Exception::translateErrorCode(HRESULT _hResult) noexcept {

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
		return "Unidentified error code";
	// copy error string from windows-allocated buffer to std::string
	std::wstring wst(pMsgBuf);
	std::string errorString(wst.begin(), wst.end());
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;

}

HRESULT Graphics::Exception::getErrorCode() const noexcept {
	return hResult;
}

std::string Graphics::Exception::getErrorString() const noexcept {
	return translateErrorCode(hResult);
}

Graphics::Graphics(HWND _hWnd) : device(nullptr), deviceContext(nullptr), swapChain(nullptr) {
	DXGI_SWAP_CHAIN_DESC desc = {}; // SwapChain descriptor/configuration

	// 0 is basically default, or "choose what is already there"
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // BGRA ? Maybe ARGB later ? we'll see
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferDesc.RefreshRate.Denominator = 0;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = _hWnd;
	desc.Windowed = TRUE;
	//desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//desc.BufferCount = 1; // Number of back buffer
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // Best performance ? It seems !
	desc.BufferCount = 2; // Number of back buffer
	desc.Flags = 0;

	namespace wrl = Microsoft::WRL;

	HRESULT hr;

	GFX_THROW_FAILED(
		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG, // to change for release mode
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&desc,
			&swapChain,
			&device,
			nullptr,
			&deviceContext
		)
	);

	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer_ptr = nullptr;
	GFX_THROW_FAILED( swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer_ptr));
	GFX_THROW_FAILED( device->CreateRenderTargetView(backBuffer_ptr.Get(), nullptr, &renderTarget));
}

void Graphics::endFrame() {
	HRESULT hr;

	if (FAILED(hr = swapChain->Present((unsigned int)1, (unsigned int)0)))
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
			throw GFX_DEVICE_REMOVED_EXCEPT(hr);
		else 
			GFX_THROW_FAILED(hr);
}

void Graphics::clearBuffer(float _r, float _g, float _b) {
	float color[4] = {_r, _g, _b, 1.0f};
	deviceContext->ClearRenderTargetView(renderTarget.Get(), color);
	
	//renderTarget.
}