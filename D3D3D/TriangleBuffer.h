#pragma once
#include <DirectXMath.h>

template <size_t T>
struct TriangleBuffer
{
public: //DirectX::XMFLOAT3;
    DirectX::XMFLOAT4 a[T]; // x, y, z and dummy
    DirectX::XMFLOAT4 ab[T];
    DirectX::XMFLOAT4 ac[T];
    DirectX::XMFLOAT4 color[T];

    size_t length = T;
};