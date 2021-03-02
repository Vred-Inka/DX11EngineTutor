#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
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

        if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
        {
            std::string outmsg = "X: ";
            outmsg += std::to_string(me.GetPosX());
            outmsg += ", Y: ";
            outmsg += std::to_string(me.GetPosY());
            outmsg += "\n";
            OutputDebugStringA(outmsg.c_str());
        }
    }
}
