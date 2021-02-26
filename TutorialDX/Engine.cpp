#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
    return this->mRenderWindow.Initialize(this, hInstance, windowTitle, windowClass, width, height);
}

bool Engine::ProcessMessages()
{
    return this->mRenderWindow.ProcessMessages();
}