#include "Graphics.h"
#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
// Basically some dark magic

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

Graphics::Graphics(HWND _hWnd, int _w, int _h) : device(nullptr), deviceContext(nullptr), swapChain(nullptr), w(_w), h(_h) {
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

	// direct3d Pipeline

	constantDesc = {};
	constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantDesc.Usage = D3D11_USAGE_DEFAULT;
	constantDesc.CPUAccessFlags = 0;
	constantDesc.MiscFlags = 0;
	constantDesc.ByteWidth = sizeof(shared);
	constantDesc.StructureByteStride = sizeof(C_Shared);
	//constantDesc.StructureByteStride = 0;

	constantSubResourceData.pSysMem = shared;

	vertexDesc = {};
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Kind of buffer
	vertexDesc.Usage = D3D11_USAGE_DEFAULT; // Where to store data
	vertexDesc.CPUAccessFlags = 0; // Can't be accessed by CPU
	vertexDesc.MiscFlags = 0; // Don't care for now
	vertexDesc.ByteWidth = sizeof(vertices); // Size of the whole data
	vertexDesc.StructureByteStride = sizeof(Vertex); // Size of one data

	vertexSubResourceData = {}; // Data holder
	vertexSubResourceData.pSysMem = vertices;

	GFX_THROW_FAILED(device->CreateBuffer(&vertexDesc, &vertexSubResourceData, &vertexBuffer_ptr));
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;
	deviceContext->IASetVertexBuffers(
		0,
		1,
		vertexBuffer_ptr.GetAddressOf(),
		&stride,
		&offset
	);

	GFX_THROW_FAILED(device->CreateBuffer(&constantDesc, &constantSubResourceData, &constantBuffer_ptr));
	deviceContext->PSSetConstantBuffers(0, 1, constantBuffer_ptr.GetAddressOf());

	triangles.triangles_points.reserve(triangles.length);
	
	// Triangles things

	triangles.triangles_points.push_back(0);
	triangles.triangles_points.push_back(0);
	triangles.triangles_points.push_back(5);

	triangles.triangles_points.push_back(1);
	triangles.triangles_points.push_back(0);
	triangles.triangles_points.push_back(0);

	triangles.triangles_points.push_back(0);
	triangles.triangles_points.push_back(1);
	triangles.triangles_points.push_back(0);
	triangles.color[0] = DirectX::XMFLOAT4(1, 0, 1, 1);
	triangle_count++;

	//triangles.a[0] = DirectX::XMFLOAT4(0, 0, 5, 0);
	//triangles.ab[0] = DirectX::XMFLOAT4(1, 0, 0, 0);
	//triangles.ac[0] = DirectX::XMFLOAT4(0, 1, 0, 0);

	//triangles.a[1] = DirectX::XMFLOAT4(0, 1, 10, 0);
	//triangles.ab[1] = DirectX::XMFLOAT4(1, 0, 0, 0);
	//triangles.ac[1] = DirectX::XMFLOAT4(0, 1, 0, 0);
	//triangles.color[1] = DirectX::XMFLOAT4(0, 0, 1, 1);
	//triangles.ab[1] = DirectX::XMFLOAT4(0.9, 0.1, 0.9, 0); // Purple
	//triangles.ac[0] = DirectX::XMFLOAT4(0.9, 0.9, 0.1, 0); // Yellow
	//triangles.ac[1] = DirectX::XMFLOAT4(0.9, 0.9, 0.9, 0); // Black

	//triangle_count++;

	triangleDesc = {};
	triangleDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Kind of buffer
	triangleDesc.Usage = D3D11_USAGE_DEFAULT; // Where to store data
	triangleDesc.CPUAccessFlags = 0; // Can't be accessed by CPU
	triangleDesc.MiscFlags = 0; // Don't care for now
	triangleDesc.ByteWidth = sizeof(triangles) - sizeof(triangles.length); // Size of the whole data
	triangleDesc.StructureByteStride = triangles.length; // Size of one data

	triangleSubResourceData = {}; // Data holder for triangles
	triangleSubResourceData.pSysMem = &triangles;

	GFX_THROW_FAILED(device->CreateBuffer(&triangleDesc, &triangleSubResourceData, &triangleBuffer_ptr));
	deviceContext->PSSetConstantBuffers(1, 1, triangleBuffer_ptr.GetAddressOf());
	//

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Microsoft::WRL::ComPtr<ID3DBlob> blob_ptr; // Read the file

	// 3. Pixel Shader : set pixel shader

	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader_ptr;
	GFX_THROW_FAILED(D3DReadFileToBlob(L"PixelShader.cso", &blob_ptr));
	GFX_THROW_FAILED(device->CreatePixelShader(blob_ptr->GetBufferPointer(), blob_ptr->GetBufferSize(), nullptr, &pixelShader_ptr)); // Read the file (we give its location + length) and load it in the pixel shader
	deviceContext->PSSetShader(pixelShader_ptr.Get(), nullptr, 0);

	// 2. Vertex Shader : Set vertex shader

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader_ptr; // Where we'll put the shader

	GFX_THROW_FAILED(D3DReadFileToBlob(L"VertexShader.cso", &blob_ptr));
	GFX_THROW_FAILED(device->CreateVertexShader(blob_ptr->GetBufferPointer(), blob_ptr->GetBufferSize(), nullptr, &vertexShader_ptr)); // Read the file (we give its location + length) and load it in the vertex shader
	deviceContext->VSSetShader(vertexShader_ptr.Get(), nullptr, 0);

	GFX_THROW_FAILED(device->CreateInputLayout(inputLayerDesc, (UINT)std::size(inputLayerDesc), blob_ptr->GetBufferPointer(), blob_ptr->GetBufferSize(), &inputLayer_ptr));
	deviceContext->IASetInputLayout(inputLayer_ptr.Get());

	D3D11_INPUT_ELEMENT_DESC d;
	//d.

	// 6. Set viewport (rasterizer stage in pipeline)
	// UPADTE IF WINDOW SIZE CHANGES
	viewPort.Width = w;
	viewPort.Height = h;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	deviceContext->RSSetViewports(1, &viewPort);
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
}

void Graphics::updateTriangles() {
	for (int i = 0; i < triangle_count; i++) {
		int offset = 9 * i;

		triangles.a[i] = DirectX::XMFLOAT4(
			triangles.triangles_points[offset],
			triangles.triangles_points[offset + 1],
			triangles.triangles_points[offset + 2],
			0);

		triangles.ab[i] = DirectX::XMFLOAT4(
			triangles.triangles_points[offset + 3],
			triangles.triangles_points[offset + 4],
			triangles.triangles_points[offset + 5], 
			0);

		triangles.ac[i] = DirectX::XMFLOAT4(
			triangles.triangles_points[offset + 6],
			triangles.triangles_points[offset + 7],
			triangles.triangles_points[offset + 8],
			0);

		//triangles.color[i] = DirectX::XMFLOAT4(0, 1, 1, 0);			//triangles.a[1] = DirectX::XMFLOAT4(0, 1, 10, 0);
	}
}

void Graphics::draw(float time, float* cameraPosition, float* cameraRotation) {
	// DIRECTX11 PIPELINE
	// 1. Input Assembler : Set the vertices

	updateTriangles();

	HRESULT hr;

	DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity()
		* DirectX::XMMatrixTranslation(cameraPosition[0], cameraPosition[2], cameraPosition[1])
		* DirectX::XMMatrixRotationX(cameraRotation[0])
		* DirectX::XMMatrixRotationY(cameraRotation[1])
		* DirectX::XMMatrixRotationZ(cameraRotation[2])
		//* DirectX::XMMatrixRotationY(time)
		//* DirectX::XMMatrixRotationZ(time)
		//* DirectX::XMMatrixScaling(480.0f / 640.0f, 1.0f, 1.0f)
		//* DirectX::XMMatrixTranslation(0, 0, 5)
		* DirectX::XMMatrixPerspectiveLH(1.0f, (float)h / (float)w, 0.5f, 10.0)

		//* DirectX::XMMatrixLookToLH(eyePos, direction, up)
		//DirectX::XMMatrixIdentity() *
		//* DirectX::XMMatrixRotationY(time)
		;

	inverseTransformMatrix = DirectX::XMMatrixInverse(nullptr, mat);
	shared[0] = {
		DirectX::XMMatrixTranspose(inverseTransformMatrix),
		triangle_count
	};

	// Update shared resources every frame
	deviceContext->UpdateSubresource( 
		constantBuffer_ptr.Get(),
		0, 
		nullptr, 
		shared,
		0,
		0);

	// Later when updating trianglePosition
	deviceContext->UpdateSubresource(
		triangleBuffer_ptr.Get(),
		0,
		nullptr,
		&triangles,
		0,
		0);

	// END VV

	// 4. Set Render Target : Where to render ?
	deviceContext->OMSetRenderTargets(1, renderTarget.GetAddressOf(), nullptr);

	deviceContext->Draw(std::size(vertices), 0);
}