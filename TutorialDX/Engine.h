#pragma once
#include "WindowContainer.h"
#include "Timer.h"

class Engine : WindowContainer
{
public:
    bool Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int with, int height);
    bool ProcessMessages();

    void Update();
    void RenderFrame();

    float mTheta;
    float mPhi;
    float mRadius;

private:
    Timer mTimer;
};
