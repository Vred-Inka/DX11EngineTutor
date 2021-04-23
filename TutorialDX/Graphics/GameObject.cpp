#include "GameObject.h"

bool GameObject::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
    if (!mModel.Initialize(filePath, device, deviceContext, texture, cb_vs_vertexshader))
    {
        return false;
    }

    this->SetPosition(0.0f, 0.0f, 0.0f);
    this->SetRotation(0.0f, 0.0f, 0.0f);
    this->UpdateWorldMatrix();

    return true;
}

void GameObject::Draw(const XMMATRIX & viewProjectionMatrix)
{
    mModel.Draw(this->mWorldMatrix, viewProjectionMatrix);
}

void GameObject::UpdateWorldMatrix()
{
    this->mWorldMatrix = XMMatrixRotationRollPitchYaw(mRot.x, mRot.y, mRot.z) * XMMatrixTranslation(mPos.x, mPos.y, mPos.z);

    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->mRot.y, 0.0f);
    this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}

const XMVECTOR & GameObject::GetPositionVector() const
{
    return this->mPosVector;
}

const XMFLOAT3 & GameObject::GetPositionFloat3() const
{
    return this->mPos;
}

const XMVECTOR & GameObject::GetRotationVector() const
{
    return this->mRotVector;
}

const XMFLOAT3 & GameObject::GetRottionFloat3() const
{
    return this->mRot;
}

void GameObject::SetPosition(const XMVECTOR & pos)
{
    XMStoreFloat3(&this->mPos, pos);
    this->mPosVector = pos;
    UpdateWorldMatrix();
}

void GameObject::SetPosition(const XMFLOAT3 & pos)
{
    this->mPos = pos;
    this->mPosVector = XMLoadFloat3(&this->mPos);
    UpdateWorldMatrix();
}

void GameObject::SetPosition(float x, float y, float z)
{
    this->mPos = XMFLOAT3(x, y, z);
    this->mPosVector = XMLoadFloat3(&this->mPos);
    UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const XMVECTOR & pos)
{
    this->mPosVector += pos;
    XMStoreFloat3(&this->mPos, this->mPosVector);
    UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const XMFLOAT3 & pos)
{
    this->mPos.x += pos.x;
    this->mPos.y += pos.y;
    this->mPos.z += pos.z;
    this->mPosVector = XMLoadFloat3(&this->mPos);
    UpdateWorldMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z)
{
    this->mPos.x += x;
    this->mPos.y += y;
    this->mPos.z += z;
    this->mPosVector = XMLoadFloat3(&this->mPos);
    UpdateWorldMatrix();
}

void GameObject::SetRotation(const XMVECTOR & rot)
{
    XMStoreFloat3(&this->mRot, rot);
    this->mRotVector = rot;
    UpdateWorldMatrix();
}

void GameObject::SetRotation(const XMFLOAT3 & rot)
{
    this->mRot = rot;
    this->mRotVector = XMLoadFloat3(&this->mRot);
    UpdateWorldMatrix();
}

void GameObject::SetRotation(float x, float y, float z)
{
    this->mRot = XMFLOAT3(x, y, z);
    this->mRotVector = XMLoadFloat3(&this->mRot);
    UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const XMVECTOR & rot)
{
    this->mRotVector += rot;
    XMStoreFloat3(&this->mRot, this->mRotVector);
    UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const XMFLOAT3 & rot)
{
    this->mRot.x += rot.x;
    this->mRot.y += rot.y;
    this->mRot.z += rot.z;
    this->mRotVector = XMLoadFloat3(&this->mRot);
    UpdateWorldMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z)
{
    this->mRot.x += x;
    this->mRot.y += y;
    this->mRot.z += z;
    this->mRotVector = XMLoadFloat3(&this->mRot);
    UpdateWorldMatrix();
}

void GameObject::SetLookAtPos(XMFLOAT3 lookAtPos)
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

const XMVECTOR & GameObject::GetForwardVector()
{
    return this->vec_forward;
}

const XMVECTOR & GameObject::GetRightVector()
{
    return this->vec_right;
}

const XMVECTOR & GameObject::GetLeftVector()
{
    return this->vec_left;
}

const XMVECTOR & GameObject::GetBackwardVector()
{
    return this->vec_backward;
}
