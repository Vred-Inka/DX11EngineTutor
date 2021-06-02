#pragma once
#include <DirectXMath.h>

struct Vertex3D
{
    Vertex3D() {};
    Vertex3D(float x, float y, float z, float u, float v, float nx, float ny, float nz)
        :mPos(x, y, z), mTexCoord(u,v), mNormal(nx, ny, nz)
    {
    }

    DirectX::XMFLOAT3 mPos;
    DirectX::XMFLOAT2 mTexCoord;
    DirectX::XMFLOAT3 mNormal;
};

struct Vertex2D
{
    Vertex2D() {};
    Vertex2D(float x, float y, float z, float u, float v)
        :mPos(x, y, z), mTexCoord(u, v)
    {
    }

    DirectX::XMFLOAT3 mPos;
    DirectX::XMFLOAT2 mTexCoord;
};


