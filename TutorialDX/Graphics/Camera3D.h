#pragma once
#include "GameObject3D.h"
using namespace DirectX;

class Camera3D: public GameObject3D
{
public:
    Camera3D();
    void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

    const XMMATRIX& GetViewMatrix() const;
    const XMMATRIX& GetProjectionMatrix() const;
    const XMMATRIX& GetViewProjectionMatrix() const;
    
private:
    void UpdateMatrix() override;

    XMMATRIX mViewMatrix;
    XMMATRIX mProjectionMatrix;
};
