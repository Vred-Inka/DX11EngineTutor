#pragma once
#include <DirectXMath.h>

struct Vertex
{
    Vertex() {};
    Vertex(float x, float y, float z, float r, float g, float b, float a)
        :mPos(x, y, z), mColor(r, g, b, a)
    {
    }

    DirectX::XMFLOAT3 mPos;
    DirectX::XMFLOAT4 mColor;
};

