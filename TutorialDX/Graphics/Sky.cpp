#include "Sky.h"
#include "GeometryGenerator.h"
#include "Camera3D.h"
#include "Vertex.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <wrl/client.h>

using namespace DirectX;

bool Sky::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float skySphereRadius)
{
    mDevice = device;
    mDeviceContext = deviceContext;

    HRESULT hr = mSkyConstantBuffer.Initialize(device, deviceContext);
    COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

    //std::wstring cubemapFilename =L"Data/Textures/sunsetcube1024.dds";
    //std::wstring cubemapFilename =L"Data/Textures/grasscube1024.dds";
    std::wstring cubemapFilename = L"Data/Textures/sky.dds";

    std::string s(cubemapFilename.begin(), cubemapFilename.end());
    std::string ex = StringHelper::GetFileExtension(s);
    if (ex == "dds")
    {
        hr = DirectX::CreateDDSTextureFromFile(device, cubemapFilename.c_str(), mCubeTexture.GetAddressOf(), mCubeTextureView.GetAddressOf());
    }
    else
    {
        hr = DirectX::CreateWICTextureFromFile(device, cubemapFilename.c_str(), mCubeTexture.GetAddressOf(), mCubeTextureView.GetAddressOf());
    }
    COM_ERROR_IF_FAILED(hr, "Failed to initialize sky cube texture.");

    
    GeometryGenerator::MeshData sphere;
    GeometryGenerator geoGen;
    geoGen.CreateSphere(skySphereRadius, 30, 30, sphere);

    std::vector<Vertex3D> vertices(sphere.mVertices.size());

    for (size_t i = 0; i < sphere.mVertices.size(); ++i)
    {
        vertices[i].mPos = sphere.mVertices[i].mPos;
    }

    mSkyVertexBuffer.Initialize(device, vertices.data(), vertices.size());
    mIndexCount = sphere.mIndices.size();

    std::vector<DWORD> indices16;
    indices16.assign(sphere.mIndices.begin(), sphere.mIndices.end());

    mSkyIndexBuffer.Initialize(device, indices16.data(), mIndexCount);

    D3D11_INPUT_ELEMENT_DESC layout3D[] =
    {
        {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    UINT numElements3D = ARRAYSIZE(layout3D);
       
    if (!mSkyVertexShader.Initialize(mDevice, L"..\\Debug\\SkyVertexShader.cso", layout3D, numElements3D))
    {
        return false;
    }

    if (!mSkyPixelShader.Initialize(mDevice, L"..\\Debug\\SkyPixelShader.cso"))
    {
        return false;
    }

    //Create sampler description for sampler state
    CD3D11_SAMPLER_DESC skysampDesc(D3D11_DEFAULT);
    skysampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    skysampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    skysampDesc.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    hr = this->mDevice->CreateSamplerState(&skysampDesc, this->mSkySamplerState.GetAddressOf()); //Create sampler state
    COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");

    CD3D11_RASTERIZER_DESC rasterizerDesc_CullNone(D3D11_DEFAULT);
    rasterizerDesc_CullNone.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    hr = this->mDevice->CreateRasterizerState(&rasterizerDesc_CullNone, this->mRasterizerState_CullNone.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

    CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    hr = this->mDevice->CreateDepthStencilState(&depthStencilDesc, this->mDepthStencilState.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create depth Sencil state.");

    return true;
}

void Sky::Draw(ID3D11DeviceContext* dc, const Camera3D& camera)
{
    // center Sky about eye in world space
    XMFLOAT3 eyePos = camera.GetPositionFloat3();
    XMMATRIX T = XMMatrixTranslation(eyePos.x, eyePos.y, eyePos.z);

    dc->VSSetShader(mSkyVertexShader.GetShader(), NULL, 0);
    dc->PSSetShader(mSkyPixelShader.GetShader(), NULL, 0);

    dc->PSSetSamplers(0, 1, mSkySamplerState.GetAddressOf());
    dc->RSSetState(mRasterizerState_CullNone.Get());
    dc->OMSetDepthStencilState(mDepthStencilState.Get(), 0);

    //XMMATRIX WVP = XMMatrixMultiply(T, camera.GetViewProjectionMatrix());
    dc->VSSetConstantBuffers(0, 1, mSkyConstantBuffer.GetAddressOf());
    mSkyConstantBuffer.data.wvpMatrix = /*XMMatrixIdentity()*/ T * camera.GetViewProjectionMatrix();
    dc->PSSetShaderResources(0, 1, mCubeTextureView.GetAddressOf());

    UINT stride = sizeof(XMFLOAT3);
    UINT offset = 0;
    dc->IASetVertexBuffers(0, 1, mSkyVertexBuffer.GetAddressOf(), mSkyVertexBuffer.StridePtr(), &offset);
    dc->IASetIndexBuffer(mSkyIndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

    mSkyConstantBuffer.ApplyChanges();
    dc->DrawIndexed(mIndexCount, 0, 0);
}