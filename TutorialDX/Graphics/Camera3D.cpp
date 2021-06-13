#include "Camera3D.h"

Camera3D::Camera3D()
{
    this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->posVector = XMLoadFloat3(&this->pos);
    this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateMatrix();
}

void Camera3D::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
    this->mProjectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX & Camera3D::GetViewMatrix() const
{
    return this->mViewMatrix;
}

const XMMATRIX & Camera3D::GetProjectionMatrix() const
{
    return this->mProjectionMatrix;
}

const XMMATRIX & Camera3D::GetViewProjectionMatrix() const
{
    return GetViewMatrix() * GetProjectionMatrix();//XMMatrixMultiply(mViewMatrix, mProjectionMatrix);
}

void Camera3D::UpdateMatrix() //Updates view matrix and also updates the movement vectors
{
    //Calculate Camera3D rotation matrix
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
    //Calculate unit vector of cam target based off Camera3D forward value transformed by cam rotation matrix
    XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
    //Adjust cam target to be offset by the Camera3D's current position
    camTarget += this->posVector;
    //Calculate up direction based on current rotation
    XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
    //Rebuild view matrix
    this->mViewMatrix = XMMatrixLookAtLH(this->posVector, camTarget, upDir);

    this->UpdateDirectionVectors();
}