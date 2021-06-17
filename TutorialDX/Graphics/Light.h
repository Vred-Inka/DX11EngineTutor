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

struct Material
{
    Material() { ZeroMemory(this, sizeof(this)); }
    DirectX::XMFLOAT4 Ambient;
    DirectX::XMFLOAT4 Diffuse;
    DirectX::XMFLOAT4 Specular;
    DirectX::XMFLOAT3 Reflect;
};

struct DirectionalLight
{
    DirectionalLight() { ZeroMemory(this, sizeof(this)); }
    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;
    XMFLOAT3 Direction;
    float Pad; // Pad the last float so we can
    // array of lights if we wanted.
};
struct PointLight
{
    PointLight() { ZeroMemory(this, sizeof(this)); }
    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;
    // Packed into 4D vector: (Position, Range)
    XMFLOAT3 Position;
    float Range;
    // Packed into 4D vector: (A0, A1, A2, Pad)
    XMFLOAT3 Att;
    float Pad; // Pad the last float so we can set an
    // array of lights if we wanted.
};
struct SpotLight
{
    SpotLight() { ZeroMemory(this, sizeof(this)); }
    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;// Packed into 4D vector: (Position, Range)
    XMFLOAT3 Position;
    float Range;
    // Packed into 4D vector: (Direction, Spot)
    XMFLOAT3 Direction;
    float Spot;
    // Packed into 4D vector: (Att, Pad)
    XMFLOAT3 Att;
    float Pad; // Pad the last float so we can set an
    // array of lights if we wanted.
};