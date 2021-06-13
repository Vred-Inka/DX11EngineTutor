#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Vertex3D>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures, const DirectX::XMMATRIX& transformMatrix)
{
    mDeviceContext = deviceContext;
    mTextures = textures;
    mTransformMatrix = transformMatrix;

    HRESULT hr = mVertexBuffer.Initialize(device, vertices.data(), vertices.size());
    COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");
    
    hr = mIndexBuffer.Initialize(device, indices.data(), indices.size());
    COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");
}

Mesh::Mesh(const Mesh & mesh)
{
    mDeviceContext = mesh.mDeviceContext;
    mIndexBuffer = mesh.mIndexBuffer;
    mVertexBuffer = mesh.mVertexBuffer;
    mTextures = mesh.mTextures;
    mTransformMatrix = mesh.mTransformMatrix;
}

void Mesh::Draw()
{
    UINT offset = 0;

    for (int i = 0; i < mTextures.size(); i++)
    {
        if (mTextures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE)
        {
            this->mDeviceContext->PSSetShaderResources(0, 1, mTextures[i].GetTextureResourceViewAdress());
            break;
        }
    }

    mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.StridePtr(), &offset);
    mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
    mDeviceContext->DrawIndexed(mIndexBuffer.IndexCount(), 0, 0);
}

const DirectX::XMMATRIX & Mesh::GetTransformMatrix()
{
    return mTransformMatrix;
}
