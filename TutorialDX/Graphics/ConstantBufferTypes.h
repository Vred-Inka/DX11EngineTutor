#pragma once
#include <DirectXMath.h>
#include "LightHelper.h"

struct CB_VS_vertexshader
{
    DirectX::XMMATRIX wvpMatrix;
    DirectX::XMMATRIX worldMatrix;
};

struct CB_VS_SkyVertexshader
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
    /*DirectX::XMFLOAT3 dynamicLightColor;//12
    float dynamicLightStrength;//4

    DirectX::XMFLOAT3 dynamicLightPosition;//12
    float mDynamicLightAttenuation_a;

    DirectX::XMFLOAT3 dirLightAmbient;
    float mDynamicLightAttenuation_b;

    DirectX::XMFLOAT3 dirlightDiffuse;
    float mDynamicLightAttenuation_c;
    */
           
    Material gMaterial;
    
    DirectionalLight gDirLight;
    PointLight gPointLight;
    SpotLight gSpotLight;
    DirectX::XMFLOAT3  gEyePosW;
    float pad;
};

