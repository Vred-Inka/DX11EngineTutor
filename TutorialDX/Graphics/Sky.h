#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Shaders.h"

class Camera3D;

class Sky 
{
public:
    bool Initialize(ID3D11Device* device, const std::wstring& cubemapFilename, float skySphereRadius, ConstantBuffer<CB_VS_SkyVertexshader>& cb_vs_vertexshader);

    ID3D11ShaderResourceView* CubeMapSRV();

    void Draw(ID3D11DeviceContext* dc, const Camera3D& camera);

private:
   ConstantBuffer<CB_VS_SkyVertexshader>* mSkyVertexShader = nullptr;
    IndexBuffer mSkyIndexBuffer;
    VertexBuffer<Vertex3D> mSkyVertexBuffer;

   //ID3D11Buffer* mVB;
  //  ID3D11Buffer* mIB;


    Microsoft::WRL::ComPtr<ID3D11Resource> mTexture = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTextureView = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mCubeMapSRV;

    UINT mIndexCount;
};