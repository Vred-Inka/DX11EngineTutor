#include "RenderableGameObject.h"

bool RenderableGameObject::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
    if (!mModel.Initialize(filePath, device, deviceContext, cb_vs_vertexshader))
    {
        return false;
    }

    this->SetPosition(0.0f, 0.0f, 0.0f);
    this->SetRotation(0.0f, 0.0f, 0.0f);
    this->UpdateMatrix();

    return true;
}

void RenderableGameObject::Draw(const XMMATRIX & viewProjectionMatrix)
{
    mModel.Draw(this->mWorldMatrix, viewProjectionMatrix);
}

void RenderableGameObject::UpdateMatrix()
{
    this->mWorldMatrix = XMMatrixRotationRollPitchYaw(mRot.x, mRot.y, mRot.z) * XMMatrixTranslation(mPos.x, mPos.y, mPos.z);

    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->mRot.y, 0.0f);
    this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}
