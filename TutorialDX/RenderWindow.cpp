#include "RenderWindow.h"

bool RendeWindow::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
    this->mhInstance = hInstance;
    this->mWidth = width;
    this->mHeight = height;
    this->mWindowTitle = windowTitle;
    this->mWindowClassWide = StringConverter::StringToWide(this->mWindowTitle);
    this->mWindowClass = windowClass;
    this->mWindowClassWide = StringConverter::StringToWide(this->mWindowClass);

    this->RegisterWindowClass();

    this->mHandle = CreateWindowEx(0,
        this->mWindowClassWide.c_str(),
        this->mWindowTitleWide.c_str(),
            WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
            0,
            0,
            this->mWidth,
            this->mHeight,
            NULL,
            NULL,
            this->mhInstance,
            nullptr);

    if (this->mHandle == NULL)
    {
        ErrorLogger::Log(GetLastError(), "CreateWindowEx Failed for window: " + this->mWindowTitle);
        return false;
    }

    ShowWindow(this->mHandle, SW_SHOW);
    SetForegroundWindow(this->mHandle);
    SetFocus(this->mHandle);

    return false;
}

bool RendeWindow::ProcessMessages()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    if (PeekMessage(&msg, this->mHandle, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg.message == WM_NULL) // WM_QUIT
    {
        if (!IsWindow(this->mHandle))
        {
            this->mHandle = NULL;
            UnregisterClass(this->mWindowClassWide.c_str(), this->mhInstance);
            return false;
        }
    }

    return true;
}

void RendeWindow::RegisterWindowClass()
{
    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = DefWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = this->mhInstance;
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = this->mWindowClassWide.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);
    RegisterClassEx(&wc);
}

RendeWindow::~RendeWindow()
{
    if (this->mHandle != NULL)
    {
        UnregisterClass(this->mWindowClassWide.c_str(), this->mhInstance);
        DestroyWindow(mHandle);
    }
}