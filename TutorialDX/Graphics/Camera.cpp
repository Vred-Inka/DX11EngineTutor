#include "Camera.h"

Camera::Camera()
{
    this->mPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->mPosVector = XMLoadFloat3(&this->mPos);
    this->mRot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->mRotVector = XMLoadFloat3(&this->mRot);
    UpdateMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
    this->mProjectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX & Camera::GetViewMatrix()
{
    return this->mViewMatrix;
}

const XMMATRIX & Camera::GetProjectionMatrix()
{
    return this->mProjectionMatrix;
}

void Camera::UpdateMatrix()
{
    //Calculate camera rotation matrix
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->mRot.x, this->mRot.y, this->mRot.z);
    //calculate unit vector of cam target based off camera forward value transformed by cam rotation
    XMVECTOR cameraTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
    //Adjust cam target to be offset by the camera's current position
    cameraTarget += this->mPosVector;
    //calculare up directtion based on current rotation
    XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
    //REbuild view matrix
    this->mViewMatrix = XMMatrixLookAtLH(this->mPosVector, cameraTarget, upDir);

    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->mRot.y, 0.0f);
    this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}
