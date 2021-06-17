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
    bool Initialize(ID3D11Device* device, ID3D11DeviceContext * deviceContext, float skySphereRadius);
    void Draw(ID3D11DeviceContext* dc, const Camera3D& camera);

private:
    ConstantBuffer<CB_VS_SkyVertexshader> mSkyConstantBuffer;
    IndexBuffer mSkyIndexBuffer;
    VertexBuffer<Vertex3D> mSkyVertexBuffer;

    PixelShader mSkyPixelShader;
    VertexShader mSkyVertexShader;

    Microsoft::WRL::ComPtr<ID3D11Resource> mCubeTexture = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mCubeTextureView = nullptr;

    Microsoft::WRL::ComPtr <ID3D11Device> mDevice = nullptr;
    Microsoft::WRL::ComPtr <ID3D11DeviceContext> mDeviceContext = nullptr;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> mSkySamplerState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState_CullNone;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;

    UINT mIndexCount;
};