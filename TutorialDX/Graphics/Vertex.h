#pragma once
#include <DirectXMath.h>

struct Vertex
{
    Vertex() {};
    Vertex(float x, float y)
        :mPos(x, y)
    {
    }

    DirectX::XMFLOAT2 mPos;
};

