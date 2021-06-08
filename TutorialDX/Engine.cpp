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
    XMFLOAT3 goPos = mgfx.mLight.GetRotationFloat3();
    float goPosition[3] = { goPos.x, goPos.y, goPos.z };

    float r = 30;
    float fi = XM_PI * 2.0f;
    float t = dt * 0.001F;

    static float step = 0.0f;


    //mgfx.mLight.AdjustPosition(0.0045f * dt, 0.0f, 0.005f * goPos.x * dt );
    //mgfx.mLight.AdjustPosition(0.0f, 0.0f, 0.05f * std::pow(t,3) - 0.01 * std::pow(t,4) );
    mgfx.mLight.SetPosition(r * std::cos(step), 5.0f, r * std::sin(step));

    if (mgfx.mLight.GetPositionFloat3().x > 1000.0 || mgfx.mLight.GetPositionFloat3().y > 1000 ||
        mgfx.mLight.GetPositionFloat3().y > 30.0 || mgfx.mLight.GetPositionFloat3().y < 0.0||
        mgfx.mLight.GetPositionFloat3().z > 30.0 || mgfx.mLight.GetPositionFloat3().z < -30.0)
    {
       // mgfx.mLight.SetPosition(5.0f, 5.0f, 0.0f);
    }

    step += 0.001f;
    if (step > 6.3f)
    {
        step = 0.0f;
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

    if (mKeyboard.KeyIsPressed(VK_UP))
    {
        this->mgfx.mGameObject.AdjustPosition(this->mgfx.mCamera3D.GetForwardVector(true) * cameraSpeed * dt);
    }
    if (mKeyboard.KeyIsPressed(VK_DOWN))
    {
        this->mgfx.mGameObject.AdjustPosition(this->mgfx.mCamera3D.GetBackwardVector(true) * cameraSpeed * dt);
    }
    if (mKeyboard.KeyIsPressed(VK_LEFT))
    {
        this->mgfx.mGameObject.AdjustPosition(this->mgfx.mCamera3D.GetLeftVector() * cameraSpeed * dt);
    }
    if (mKeyboard.KeyIsPressed(VK_RIGHT))
    {
        this->mgfx.mGameObject.AdjustPosition(this->mgfx.mCamera3D.GetRightVector() * cameraSpeed * dt);
    }
    
}

void Engine::RenderFrame()
{
    mgfx.RenderFrame();
}
