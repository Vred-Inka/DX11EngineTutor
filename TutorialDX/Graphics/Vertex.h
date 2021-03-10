#pragma once
#include <DirectXMath.h>

struct Vertex
{
    Vertex() {};
    Vertex(float x, float y, float z, float u, float v)
        :mPos(x, y, z), texCoord(u,v)
    {
    }

    DirectX::XMFLOAT3 mPos;
    DirectX::XMFLOAT2 texCoord;
};

