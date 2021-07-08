#pragma once
#include "RenderableGameObject.h"

class Light : public RenderableGameObject
{
public:
    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
    DirectX::XMFLOAT3 mLightColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
    float mLightStrength = 1.0f;
    float mAttenuation_a = 1.0f;
    float mAttenuation_b = 0.1f;
    float mAttenuation_c = 0.1f;

    DirectX::XMFLOAT3 mDirLightAmbient = XMFLOAT3(0.2f, 0.2f, 0.2f);
    DirectX::XMFLOAT3 mDirlightDiffuse = XMFLOAT3(0.5f, 0.5f, 0.5f);
    DirectX::XMFLOAT3 mDirLightSpectular = XMFLOAT3(0.5f, 0.5f, 0.5f);
    DirectX::XMFLOAT3 mDirLightDirection = XMFLOAT3(0.57735f, 0.57735f, 0.57735f);
};
