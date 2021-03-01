#include "WindowContainer.h"

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYDOWN:
    {
        unsigned char keyCode = static_cast<unsigned char>(wParam);
        if (mKeyboard.IsKeysAutoRepeat())
        {
            mKeyboard.OnKeyPressed(keyCode);
        }
        else
        {
            const bool wasPressed = lParam & 0x40000000;
            if (!wasPressed)
            {
                mKeyboard.OnKeyPressed(keyCode);
            }
        }
        
        return 0; 
    }
    case WM_KEYUP:
    {
        unsigned char keyCode = static_cast<unsigned char>(wParam);
        mKeyboard.OnKeyReleased(keyCode);
        return 0;
    }
    case WM_CHAR:
    {
        unsigned char ch = static_cast<unsigned char>(wParam);
        if (mKeyboard.IsCharsAutoRepeat())
        {
            mKeyboard.OnChar(ch);
        }
        else
        {
            const bool wasPressed = lParam & 0x40000000;
            if (!wasPressed)
            {
                mKeyboard.OnChar(ch);
            }
        }
        return 0;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}