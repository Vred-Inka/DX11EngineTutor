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
             
    Material gMaterial;
    
    DirectionalLight gDirLight;
    PointLight gPointLight;
    SpotLight gSpotLight;
    DirectX::XMFLOAT3  gEyePosW;
    float gReflectionsEnabled;

    PointLight lights[100];
};

