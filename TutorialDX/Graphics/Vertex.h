#pragma once
#include <DirectXMath.h>

struct Vertex
{
    Vertex() {};
    Vertex(float x, float y, float z, DirectX::XMVECTORF32 color)
        :mPos(x, y, z), mColor(color)
    {
    }

    DirectX::XMFLOAT3 mPos;
    DirectX::XMVECTORF32 mColor;
};

