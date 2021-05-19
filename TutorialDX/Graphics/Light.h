#pragma once
#include "RenderableGameObject.h"

class Light : public RenderableGameObject
{
public:
    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
    DirectX::XMFLOAT3 mLightColor = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
    float mLightStrength = 1.0f;
    float mAttenuation_a = 1.0f;
    float mAttenuation_b = 0.1f;
    float mAttenuation_c = 0.1f;

};