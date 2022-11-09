#pragma once

#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Shaders.h"
#include "Camera3D.h"

using namespace std;

class Grid
{
    float mRowCount;
    float mDistance;
public:
    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

    VertexBuffer<Vertex3D> mVertexBuffer;
    IndexBuffer mIndexBuffer;
    ID3D11DeviceContext* mDeviceContext;
    UINT mIndexCount;

    void Draw(ID3D11DeviceContext* dc, const Camera3D& camera);
};


