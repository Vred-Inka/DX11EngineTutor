#include "Sprite.h"
#include <WICTextureLoader.h>

bool Sprite::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, float width, float height, std::string spritePath, ConstantBuffer<CB_VS_vertexshader_2d>& cb_vs_vertexshader_2d)
{
    this->mDeviceContext = deviceContext;
    if (deviceContext == nullptr)
    {
        return false;
    }

    this->cb_vs_vertexshader_2d = &cb_vs_vertexshader_2d;
    mTexture = std::make_unique<Texture>(device, spritePath, aiTextureType::aiTextureType_DIFFUSE);

    std::vector<Vertex2D> vertexData =
    {
        Vertex2D(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f),
        Vertex2D(0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
        Vertex2D(-0.5f, 0.5f, 0.0f, 0.0f, 1.0f),
        Vertex2D(0.5f, 0.5f, 0.0f, 1.0f, 1.0f)
    };

    std::vector<DWORD> indexData =
    {
        0,1,2,
        2,1,3
    };

    HRESULT hr = mVerticies.Initialize(device, vertexData.data(), vertexData.size());
    COM_ERROR_IF_FAILED(hr, "Fail to initialize buffer for sprite.");

    hr = mIndices.Initialize(device, indexData.data(), indexData.size());
    COM_ERROR_IF_FAILED(hr, "Fail to initialize buffer for mesh.");

    SetPosition(0.0f, 0.0f, 0.0f);
    SetRotation(0.0f, 0.0f, 0.0f);
    SetScale(width, height);
    return true;
}

void Sprite::Draw(XMMATRIX orthoMatrix)
{
    XMMATRIX wvpMatrix = mWorldMatrix * orthoMatrix;
    mDeviceContext->VSSetConstantBuffers(0, 1, cb_vs_vertexshader_2d->GetAddressOf());
    cb_vs_vertexshader_2d->data.wvpMatrix = wvpMatrix;
    cb_vs_vertexshader_2d->ApplyChanges();

    mDeviceContext->PSSetShaderResources(0, 1, mTexture->GetTextureResourceViewAdress());

    const UINT offsets = 0;
    mDeviceContext->IASetVertexBuffers(0, 1, mVerticies.GetAddressOf(), mVerticies.StridePtr(), &offsets);
    mDeviceContext->IASetIndexBuffer(mIndices.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
    mDeviceContext->DrawIndexed(mIndices.IndexCount(), 0, 0);
}

float Sprite::GetWidth()
{
    return scale.x;
}

float Sprite::GetHeight()
{
    return scale.y;
}

void Sprite::UpdateMatrix()
{
    mWorldMatrix = XMMatrixScaling(scale.x, scale.y, 1.0f)
        * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z)
        * XMMatrixTranslation(pos.x + scale.x / 2.0f, pos.y + scale.y / 2.0f, pos.z + scale.z / 2.0f);
}