#pragma once
#include <DirectXMath.h>

struct Vertex
{
    Vertex() {};
    Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
        :mPos(x, y, z), mTexCoord(u,v), mNormal(nx, ny, nz)
    {
    }

    DirectX::XMFLOAT3 mPos;
    DirectX::XMFLOAT2 mTexCoord;
    DirectX::XMFLOAT3 mNormal;
};

