#include "Grid.h"
#include "GeometryGenerator.h"

bool Grid::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
    GeometryGenerator::MeshData grid;
    GeometryGenerator geoGen;
    geoGen.CreateGrid(3560.0f, 3560.0f, 500, 500, grid);
    mIndexCount = grid.mIndices.size();

    mDeviceContext = deviceContext;
    for (size_t i = 0; i < grid.mVertices.size(); ++i)
    {
        //grid.mVertices[i].m = blue;
    }

    HRESULT hr = mVertexBuffer.Initialize(device, grid.mVertices.data(), grid.mVertices.size());
    COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");

    std::vector<DWORD> indices16;
    indices16.assign(grid.mIndices.begin(), grid.mIndices.end());

    hr = mIndexBuffer.Initialize(device, indices16.data(), mIndexCount);
    COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");

    return true;
}

void Grid::Draw(ID3D11DeviceContext* dc, const Camera3D& camera)
{
    UINT stride = sizeof(XMFLOAT3);
    UINT offset = 0;
    dc->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.StridePtr(), &offset);
    dc->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
    mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    dc->DrawIndexed(mIndexCount, 0, 0);
    mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}