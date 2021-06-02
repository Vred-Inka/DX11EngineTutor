#pragma once
#include <DirectXMath.h>

struct CB_VS_vertexshader
{
    DirectX::XMMATRIX wvpMatrix;
    DirectX::XMMATRIX worldMatrix;
};

struct CB_VS_vertexshader_2d
{
    DirectX::XMMATRIX wvpMatrix;
};

struct CB_PS_light
{
    DirectX::XMFLOAT3 ambientLightColor;//12
    float ambientLightStrength;//4
    //16
    DirectX::XMFLOAT3 dynamicLightColor;//12
    float dynamicLightStrength;//4
    DirectX::XMFLOAT3 dynamicLightPosition;//12

    float mDynamicLightAttenuation_a;
    float mDynamicLightAttenuation_b;
    float mDynamicLightAttenuation_c;

};

