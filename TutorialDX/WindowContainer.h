#pragma once
#include "RenderWindow.h"
#include "Keyboard/KeyboardClass.h"

class WindowContainer
{
public:
    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //Mouse
protected:
    RendeWindow mRenderWindow;
    KeyboardClass mKeyboard;

private:
};
