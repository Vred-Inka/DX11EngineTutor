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
    XMFLOAT3 goPos = mgfx.mLights[0].GetRotationFloat3();
    float goPosition[3] = { goPos.x, goPos.y, goPos.z };

    float r = 30;
    float fi = XM_PI * 2.0f;
    float t = dt * 0.001F;

    static float step = 0.0f;


    //mgfx.mLight.AdjustPosition(0.0045f * dt, 0.0f, 0.005f * goPos.x * dt );
    //mgfx.mLight.AdjustPosition(0.0f, 0.0f, 0.05f * std::pow(t,3) - 0.01 * std::pow(t,4) );
    mgfx.mLights[0].SetPosition(r * std::cos(step), 5.0f, r * std::sin(step));

    step += 0.001f;
    if (step > 6.3f)
    {
        step = 0.0f;
    }


    float x = mRadius * sinf(mPhi)*cosf(mTheta);
    float z = mRadius * sinf(mPhi)*sinf(mTheta);
    float y = mRadius * cosf(mPhi);
    mgfx.mEyePosW = mgfx.mCamera3D.GetPositionFloat3();//XMFLOAT3(x, y, z);
    // Build the view matrix.
    XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    /* ...Irrelevant code omitted ... */
    // Circle light over the land surface.
    mgfx.mPointLight.Position.x = 70.0f*cosf(0.2f*mTimer.TotalTime());
    mgfx.mPointLight.Position.z = 70.0f*sinf(0.2f*mTimer.TotalTime());
    mgfx.mPointLight.Position.y = 50.0f;// MathHelper::Max(GetHillHeight(mgfx.mPointLight.Position.x, mgfx.mPointLight.Position.z), -3.0f) + 10.0f;
    // The spotlight takes on the camera position and is aimed in the
    // same direction the camera is looking. In this way, it looks
    // like we are holding a flashlight.
    mgfx.mSpotLight.Position = mgfx.mEyePosW;
    XMStoreFloat3(&mgfx.mSpotLight.Direction,
        XMVector3Normalize(target - pos));

    float cameraSpeed = 0.006f;

    if (mKeyboard.KeyIsPressed(VK_SHIFT))
    {
        cameraSpeed = 0.6f;
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
        this->mgfx.mLights[0].SetPosition(lightPosition);
        this->mgfx.mLights[0].SetRotation(this->mgfx.mCamera3D.GetRotationFloat3());
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
