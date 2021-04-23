#pragma once
#include "Model.h"

using namespace DirectX;

class GameObject
{
public:
    bool Initialize(const std::string& filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader> & cb_vs_vertexshader);
    void Draw(const XMMATRIX & viewProjectionMatrix);

    const XMVECTOR& GetPositionVector() const;
    const XMFLOAT3& GetPositionFloat3() const;
    const XMVECTOR& GetRotationVector() const;
    const XMFLOAT3& GetRottionFloat3() const;

    void SetPosition(const XMVECTOR& pos);
    void SetPosition(const XMFLOAT3& pos);
    void SetPosition(float x, float y, float z);
    void AdjustPosition(const XMVECTOR& pos);
    void AdjustPosition(const XMFLOAT3& pos);
    void AdjustPosition(float x, float y, float z);
    void SetRotation(const XMVECTOR& rot);
    void SetRotation(const XMFLOAT3& rot);
    void SetRotation(float x, float y, float z);
    void AdjustRotation(const XMVECTOR& rot);
    void AdjustRotation(const XMFLOAT3& rot);
    void AdjustRotation(float x, float y, float z);

    void SetLookAtPos(XMFLOAT3 lookAtPos);

    const XMVECTOR& GetForwardVector();
    const XMVECTOR& GetRightVector();
    const XMVECTOR& GetLeftVector();
    const XMVECTOR& GetBackwardVector();

private:
    Model mModel;
    void UpdateWorldMatrix();
    
    XMMATRIX mWorldMatrix = XMMatrixIdentity();

    XMVECTOR mPosVector;
    XMVECTOR mRotVector;
    XMFLOAT3 mPos;
    XMFLOAT3 mRot;

    const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
    const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
    const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

    XMVECTOR vec_forward;
    XMVECTOR vec_left;
    XMVECTOR vec_right;
    XMVECTOR vec_backward;

};
