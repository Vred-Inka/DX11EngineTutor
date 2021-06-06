#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
    mTimer.Start();

    if (!this->mRenderWindow.Initialize(this, hInstance, windowTitle, windowClass, width, height))
    {
        return false;
    }

    if (!mgfx.Initialize(this->mRenderWindow.GetHWND(), width, height))
    {
        return false;
    }

    return true;
}

bool Engine::ProcessMessages()
{
    return this->mRenderWindow.ProcessMessages();
}

void Engine::Update()
{
    float dt = mTimer.GetMilisecondsElapsed();
    mTimer.Restart();

    while (!mKeyboard.CharBufferIsEmpty())
    {
        unsigned char ch = mKeyboard.ReadChar();
        std::string outmsg = "Char: ";
        outmsg += ch;
        outmsg += "\n";
        OutputDebugStringA(outmsg.c_str());
    }

    while (!mKeyboard.KeyBufferIsEmpty())
    {
        KeyboardEvent kbe = mKeyboard.ReadKey();
        unsigned char keycode = kbe.GetKeyCode();
        std::string outmsg = "";
        if (kbe.IsPress())
        {
            outmsg += "Key press: ";
        }

        if (kbe.IsRelease())
        {
            outmsg += "Key release: ";
        }

        outmsg += keycode;
        outmsg += "\n";
        OutputDebugStringA(outmsg.c_str());
    }

    while (!mMouse.EventBufferIsEmpty())
    {
        MouseEvent me = mMouse.ReadEvent();

        if (me.GetType() == MouseEvent::EventType::WheelUp)
        {
            OutputDebugStringA("MouseEvent::EventType::WheelUp\n");
        }

        if (me.GetType() == MouseEvent::EventType::WheelDown)
        {
            OutputDebugStringA("MouseEvent::EventType::WheelDown\n");
        }
        
        if (mMouse.IsRightDown())
        {
            if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
            {
                this->mgfx.mCamera3D.AdjustRotation((float)me.GetPosY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
            }
        }
    }

    mgfx.mGameObject.AdjustRotation( 0.0f, 0.001f*dt,  0.0f);

    mgfx.mSprite.AdjustPosition(0.1f* dt, 0.0f, 0.0f);
    if (mgfx.mSprite.GetPositionFloat3().x > 160.0)
    {
        mgfx.mSprite.SetPosition(0.01f, 0.0f, 0.0f);
    }

    float cameraSpeed = 0.006f;

    if (mKeyboard.KeyIsPressed(VK_SHIFT))
    {
        cameraSpeed = 0.3f;
    }

    if (mKeyboard.KeyIsPressed('W'))
    {
        this->mgfx.mCamera3D.AdjustPosition(this->mgfx.mCamera3D.GetForwardVector() * cameraSpeed * dt);
    }
    if (mKeyboard.KeyIsPressed('S'))
    {
        this->mgfx.mCamera3D.AdjustPosition(this->mgfx.mCamera3D.GetBackwardVector() * cameraSpeed * dt);
    }
    if (mKeyboard.KeyIsPressed('A'))
    {
        this->mgfx.mCamera3D.AdjustPosition(this->mgfx.mCamera3D.GetLeftVector() * cameraSpeed * dt);
    }
    if (mKeyboard.KeyIsPressed('D'))
    {
        this->mgfx.mCamera3D.AdjustPosition(this->mgfx.mCamera3D.GetRightVector() * cameraSpeed * dt);
    }
    if (mKeyboard.KeyIsPressed(VK_SPACE))
    {
        this->mgfx.mCamera3D.AdjustPosition(0.0f, cameraSpeed * dt, 0.0f);
    }
    if (mKeyboard.KeyIsPressed('Z'))
    {
        this->mgfx.mCamera3D.AdjustPosition(0.0f, -cameraSpeed * dt, 0.0f);
    }

    if (mKeyboard.KeyIsPressed('C'))
    {
        XMVECTOR lightPosition = this->mgfx.mCamera3D.GetPositionVector();
        lightPosition += this->mgfx.mCamera3D.GetForwardVector();
        this->mgfx.mLight.SetPosition(lightPosition);
        this->mgfx.mLight.SetRotation(this->mgfx.mCamera3D.GetRotationFloat3());
    }
}

void Engine::RenderFrame()
{
    mgfx.RenderFrame();
}
