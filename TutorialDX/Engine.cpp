#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
    return this->mRenderWindow.Initialize(this, hInstance, windowTitle, windowClass, width, height);
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
}
