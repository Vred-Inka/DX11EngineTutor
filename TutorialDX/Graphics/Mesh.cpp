#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Vertex>& vertices, std::vector<DWORD>& indices)
{
    mDeviceContext = deviceContext;

    HRESULT hr = mVertexBuffer.Initialize(device, vertices.data(), vertices.size());
    COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");
    
    hr = mIndexBuffer.Initialize(device, indices.data(), vertices.size());
    COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");
}

Mesh::Mesh(const Mesh & mesh)
{
    mDeviceContext = mesh.mDeviceContext;
    mIndexBuffer = mesh.mIndexBuffer;
    mVertexBuffer = mesh.mVertexBuffer;
}

void Mesh::Draw()
{
    UINT offset = 0;
    mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.StridePtr(), &offset);
    mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
    mDeviceContext->DrawIndexed(mIndexBuffer.IndexCount(), 0, 0);
}
