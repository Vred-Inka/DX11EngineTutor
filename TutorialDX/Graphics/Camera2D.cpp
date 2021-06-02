#include "Camera2D.h"

Camera2D::Camera2D()
{
    pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    posVector = XMLoadFloat3(&pos);
    rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    rotVector = XMLoadFloat3(&rot);
    UpdateMatrix();
}

void Camera2D::SetProjectionValues(float width, float height, float nearZ, float farZ)
{
    mOrthoMatrix = XMMatrixOrthographicOffCenterLH(0.0f, width, height, 0.0f, nearZ, farZ);
}

const XMMATRIX & Camera2D::GetWorldMatrix() const
{
    return mWorldMatrix;
}

void Camera2D::UpdateMatrix()
{
    XMMATRIX translationOffsetMatrix = XMMatrixTranslation(-pos.x, -pos.y, 0.0f);
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
    mWorldMatrix = camRotationMatrix * translationOffsetMatrix;
}

const XMMATRIX & Camera2D::GetOrthoMatrix() const
{
    return mOrthoMatrix;
}
