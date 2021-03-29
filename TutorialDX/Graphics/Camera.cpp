#include "Camera.h"

Camera::Camera()
{
    this->mPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->mPosVector = XMLoadFloat3(&this->mPos);
    this->mRot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->mRotVector = XMLoadFloat3(&this->mRot);
    UpdateViewMatrix();
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

const XMVECTOR & Camera::GetPositionVector() const
{
    return this->mPosVector;
}

const XMFLOAT3 & Camera::GetPositionFloat3() const
{
    return this->mPos;
}

const XMVECTOR & Camera::GetRotationVector() const
{
    return this->mRotVector;
}

const XMFLOAT3 & Camera::GetRottionFloat3() const
{
    return this->mRot;
}

void Camera::SetPosition(const XMVECTOR & pos)
{
    XMStoreFloat3(&this->mPos, pos);
    this->mPosVector = pos;
    UpdateViewMatrix();
}

void Camera::SetPosition(const XMFLOAT3 & pos)
{
    this->mPos = pos;
    this->mPosVector = XMLoadFloat3(&this->mPos);
    UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
    this->mPos = XMFLOAT3(x, y, z);
    this->mPosVector = XMLoadFloat3(&this->mPos);
    UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR & pos)
{
    this->mPosVector += pos;
    XMStoreFloat3(&this->mPos, this->mPosVector);
    UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMFLOAT3 & pos)
{
    this->mPos.x += pos.x;
    this->mPos.y += pos.y;
    this->mPos.z += pos.z;
    this->mPosVector = XMLoadFloat3(&this->mPos);
    UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
    this->mPos.x += x;
    this->mPos.y += y;
    this->mPos.z += z;
    this->mPosVector = XMLoadFloat3(&this->mPos);
    UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR & rot)
{
    XMStoreFloat3(&this->mRot, rot);
    this->mRotVector = rot;
    UpdateViewMatrix();
}

void Camera::SetRotation(const XMFLOAT3 & rot)
{
    this->mRot = rot;
    this->mRotVector = XMLoadFloat3(&this->mRot);
    UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
    this->mRot = XMFLOAT3(x, y, z);
    this->mRotVector = XMLoadFloat3(&this->mRot);
    UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR & rot)
{
    this->mRotVector += rot;
    XMStoreFloat3(&this->mRot, this->mRotVector);
    UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMFLOAT3 & rot)
{
    this->mRot.x += rot.x;
    this->mRot.y += rot.y;
    this->mRot.z += rot.z;
    this->mRotVector = XMLoadFloat3(&this->mRot);
    UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
    this->mRot.x += x;
    this->mRot.y += y;
    this->mRot.z += z;
    this->mRotVector = XMLoadFloat3(&this->mRot);
    UpdateViewMatrix();
}

void Camera::SetLookAtPos(XMFLOAT3 lookAtPos)
{
    if (lookAtPos.x == this->mPos.x && lookAtPos.y == this->mPos.y && lookAtPos.z == this->mPos.z)
    {
        return;
    }

    lookAtPos.x = this->mPos.x - lookAtPos.x;
    lookAtPos.y = this->mPos.y - lookAtPos.y;
    lookAtPos.z = this->mPos.z - lookAtPos.z;

    float pitch = 0.0f;
    if (lookAtPos.y != 0.0f)
    {
        const float distance = sqrt(lookAtPos.x * lookAtPos.x * lookAtPos.z *lookAtPos.z);
        pitch = atan(lookAtPos.y / distance);
    }

    float yaw = 0.0f;
    if (lookAtPos.x != 0.0f)
    {
        yaw = atan(lookAtPos.x / lookAtPos.z);
    }

    if (lookAtPos.z > 0)
    {
        yaw += XM_PI;
    }

    this->SetRotation(pitch, yaw, 0.0f);
}

const XMVECTOR & Camera::GetForwardVector()
{
    return this->vec_forward;
}

const XMVECTOR & Camera::GetRightVector()
{
    return this->vec_right;
}

const XMVECTOR & Camera::GetLeftVector()
{
    return this->vec_left;
}

const XMVECTOR & Camera::GetBackwardVector()
{
    return this->vec_backward;
}

void Camera::UpdateViewMatrix()
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
