#pragma once
#include "GameObject.h"
using namespace DirectX;

class Camera: public GameObject
{
public:
    Camera();
    void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

    const XMMATRIX& GetViewMatrix();
    const XMMATRIX& GetProjectionMatrix();
    
private:
    void UpdateMatrix() override;

    XMMATRIX mViewMatrix;
    XMMATRIX mProjectionMatrix;
};
