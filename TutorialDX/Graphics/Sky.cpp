#include "Sky.h"
#include "GeometryGenerator.h"
#include "Camera3D.h"
#include "Vertex.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

using namespace DirectX;

bool Sky::Initialize(ID3D11Device* device, const std::wstring& cubemapFilename, float skySphereRadius, ConstantBuffer<CB_VS_SkyVertexshader>& vertexshader)
{
    mSkyVertexShader = &vertexshader;
    std::string s(cubemapFilename.begin(), cubemapFilename.end());
    std::string ex = StringHelper::GetFileExtension(s);
    if (ex == "dds")
    {
        HRESULT hr = DirectX::CreateDDSTextureFromFile(device, cubemapFilename.c_str(), mTexture.GetAddressOf(), mTextureView.GetAddressOf());
    }
    else
    {
        HRESULT hr = DirectX::CreateWICTextureFromFile(device, cubemapFilename.c_str(), mTexture.GetAddressOf(), mTextureView.GetAddressOf());
    }


    GeometryGenerator::MeshData sphere;
    GeometryGenerator geoGen;
    geoGen.CreateSphere(skySphereRadius, 30, 30, sphere);

    std::vector<Vertex3D> vertices(sphere.mVertices.size());

    for (size_t i = 0; i < sphere.mVertices.size(); ++i)
    {
        vertices[i].mPos = sphere.mVertices[i].mPos;
    }

   // D3D11_BUFFER_DESC vbd;
   // vbd.Usage = D3D11_USAGE_IMMUTABLE;
   // vbd.ByteWidth = sizeof(XMFLOAT3) * vertices.size();
    //vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    //vbd.CPUAccessFlags = 0;
    //vbd.MiscFlags = 0;
    //vbd.StructureByteStride = 0;

   // D3D11_SUBRESOURCE_DATA vinitData;
    //vinitData.pSysMem = &vertices[0];

    mSkyVertexBuffer.Initialize(device, vertices.data(), vertices.size());
    //HRESULT(device->CreateBuffer(&vbd, &vinitData, &mSkyVertexBuffer));


    mIndexCount = sphere.mIndices.size();

  //  D3D11_BUFFER_DESC ibd;
   // ibd.Usage = D3D11_USAGE_IMMUTABLE;
   // ibd.ByteWidth = sizeof(USHORT) * mIndexCount;
    //ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
   // ibd.CPUAccessFlags = 0;
    //ibd.StructureByteStride = 0;
   // ibd.MiscFlags = 0;

    std::vector<DWORD> indices16;
    indices16.assign(sphere.mIndices.begin(), sphere.mIndices.end());

    //D3D11_SUBRESOURCE_DATA iinitData;
   // iinitData.pSysMem = &indices16[0];
    mSkyIndexBuffer.Initialize(device, indices16.data(), mIndexCount);
    //HRESULT(device->CreateBuffer(&ibd, &iinitData, &mSkyIndexBuffer));
    return true;
}

ID3D11ShaderResourceView* Sky::CubeMapSRV()
{
    return mCubeMapSRV.Get();
}

void Sky::Draw(ID3D11DeviceContext* dc, const Camera3D& camera)
{
    // center Sky about eye in world space
    XMFLOAT3 eyePos = camera.GetPositionFloat3();
    XMMATRIX T = XMMatrixTranslation(eyePos.x, eyePos.y, eyePos.z);

    XMMATRIX WVP = XMMatrixMultiply(T, camera.GetViewProjectionMatrix());

    //mSkyVertexShader->
    //Effects::SkyFX->SetWorldViewProj(WVP);
    //Effects::SkyFX->SetCubeMap(mCubeMapSRV);
    //dc->VSSetConstantBuffers(1, 1, mSkyVertexShader->GetAddressOf());
    mSkyVertexShader->data.wvpMatrix = XMMatrixIdentity() * camera.GetViewMatrix() * camera.GetProjectionMatrix();
    dc->PSSetShaderResources(0, 1, mTextureView.GetAddressOf());


    UINT stride = sizeof(XMFLOAT3);
    UINT offset = 0;
    dc->IASetVertexBuffers(0, 1, mSkyVertexBuffer.GetAddressOf(), mSkyVertexBuffer.StridePtr(), &offset);
    dc->IASetIndexBuffer(mSkyIndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
    //dc->IASetInputLayout(mSkyVertexShader.GetInputLayout());
    //dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

   // mSkyVertexShader->data.worldMatrix = camera.GetViewMatrix();
    mSkyVertexShader->ApplyChanges();

    //D3DX11_TECHNIQUE_DESC techDesc;
    //Effects::SkyFX->SkyTech->GetDesc(&techDesc);

    dc->DrawIndexed(mIndexCount, 0, 0);
}