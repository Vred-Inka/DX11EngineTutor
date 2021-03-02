#pragma once
#include <DirectXMath.h>

struct Vertex
{
    Vertex() {};
    Vertex(float x, float y, float r, float g, float b)
        :mPos(x, y), mColor(r, g, b)
    {
    }

    DirectX::XMFLOAT2 mPos;
    DirectX::XMFLOAT3 mColor;
};

