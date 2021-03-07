#pragma once
#include <DirectXMath.h>

struct Vertex
{
    Vertex() {};
    Vertex(float x, float y, float z, float r, float g, float b)
        :mPos(x, y, z), mColor(r, g, b)
    {
    }

    DirectX::XMFLOAT3 mPos;
    DirectX::XMFLOAT3 mColor;
};

