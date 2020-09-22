#pragma once
#include <DirectXMath.h>
#include <fstream>
#include <vector>

template <size_t T>
struct TriangleBuffer
{
public: //DirectX::XMFLOAT3;
    DirectX::XMFLOAT4 a[T]; // x, y, z and dummy
    DirectX::XMFLOAT4 ab[T];
    DirectX::XMFLOAT4 ac[T];
    DirectX::XMFLOAT4 color[T];

    std::vector<float> triangles_points;

    size_t length = T;
};