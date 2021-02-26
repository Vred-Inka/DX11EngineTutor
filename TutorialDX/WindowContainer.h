#pragma once
#include "RenderWindow.h"

class WindowContainer
{
public:
    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //Mouse
    //Keyboard
protected:
    RendeWindow mRenderWindow;

private:
};
