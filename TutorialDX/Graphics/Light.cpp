#include "Light.h"

bool Light::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
    if (!mModel.Initialize("Data/Objects/light.fbx", device, deviceContext, cb_vs_vertexshader))
    {
        return false;
    }

    SetPosition(0.0f, 0.0f, 0.0f);
    SetRotation(0.0f, 0.0f, 0.0f);
    UpdateMatrix();
    return true;
}