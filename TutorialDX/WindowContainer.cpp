#include "WindowContainer.h"

WindowContainer::WindowContainer()
{
    static bool rawInputInitialize = false;

    if (rawInputInitialize == false)
    {
        RAWINPUTDEVICE rid;

        rid.usUsagePage = 0x01;
        rid.usUsage = 0x02;
        rid.dwFlags = 0;
        rid.hwndTarget = NULL;
       
        if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
        {
            ErrorLogger::Log(GetLastError(), "Failed to register raw input devices.");
            exit(-1);
        }

        rawInputInitialize = true;
    }
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
    {
        return true;
    }

    switch (uMsg)
    {
        //keyboard messages
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

    //Mouse messages
    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mMouse.OnLeftPressed(x, y);
        return 0;
    }
    case WM_RBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mMouse.OnRightPressed(x, y);
        return 0;
    }
    case WM_MBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mMouse.OnMiddlePressed(x, y);
        return 0;
    }
    case WM_LBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mMouse.OnLeftReleased(x, y);
        return 0;
    }
    case WM_RBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mMouse.OnRightReleased(x, y);
        return 0;
    }
    case WM_MBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mMouse.OnMiddleReleased(x, y);
        return 0;
    }
    case WM_MOUSEWHEEL:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
        {
            mMouse.OnWheelUp(x, y);
        }
        else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
        {
            mMouse.OnWheelDown(x, y);
        }
        break;
    }
    case WM_INPUT:
    {
        UINT dataSize;
        GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

        if (dataSize > 0)
        {
            std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);

            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
            {
                RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
                if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    mMouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                }
            }
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    case WM_MOUSEMOVE:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mMouse.OnMouseMove(x, y);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}