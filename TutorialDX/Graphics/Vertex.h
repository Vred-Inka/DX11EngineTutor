#pragma once
#include <DirectXMath.h>

struct Vertex3D
{
    Vertex3D() {};
    Vertex3D(float x, float y, float z,
             float u, float v, 
             float nx, float ny, float nz,
             float tx = 1.0f, float ty = 1.0f, float tz = 1.0f)
        :mPos(x, y, z), mTexCoord(u,v), mNormal(nx, ny, nz), mTanU(tx, ty, tz)
    {
    }

    DirectX::XMFLOAT3 mPos;
    DirectX::XMFLOAT2 mTexCoord;
    DirectX::XMFLOAT3 mNormal;
    DirectX::XMFLOAT3 mTanU;
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

/*class InputLayoutDesc
{
public:
    // Init like const int A::a[4] = {0, 1, 2, 3}; in .cpp file.
    static const D3D11_INPUT_ELEMENT_DESC Pos[1];
    static const D3D11_INPUT_ELEMENT_DESC Basic32[3];
};

class InputLayouts
{
public:
    static void InitAll(ID3D11Device* device);
    static void DestroyAll();

    static ID3D11InputLayout* Pos;
    static ID3D11InputLayout* Basic32;
};*/

