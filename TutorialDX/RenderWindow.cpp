#include "WindowContainer.h"

bool RendeWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
    this->mhInstance = hInstance;
    this->mWidth = width;
    this->mHeight = height;
    this->mWindowTitle = windowTitle;
    this->mWindowClassWide = StringHelper::StringToWide(this->mWindowTitle);
    this->mWindowClass = windowClass;
    this->mWindowClassWide = StringHelper::StringToWide(this->mWindowClass);

    this->RegisterWindowClass();

    int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - this->mWidth / 2;
    int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - this->mHeight / 2;

    RECT wr;
    wr.left = centerScreenX;
    wr.top = centerScreenY;
    wr.right = wr.left + this->mWidth;
    wr.bottom = wr.top + this->mHeight;
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    this->mHandle = CreateWindowEx(0,
        this->mWindowClassWide.c_str(),
        this->mWindowTitleWide.c_str(),
            WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
            wr.left,
            wr.top,
            wr.right - wr.left,
            wr.bottom - wr.top,
            NULL,
            NULL,
            this->mhInstance,
            pWindowContainer);

    if (this->mHandle == NULL)
    {
        ErrorLogger::Log(GetLastError(), "CreateWindowEx Failed for window: " + this->mWindowTitle);
        return false;
    }

    ShowWindow(this->mHandle, SW_SHOW);
    SetForegroundWindow(this->mHandle);
    SetFocus(this->mHandle);

    return true;
}

bool RendeWindow::ProcessMessages()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while (PeekMessage(&msg, this->mHandle, 0, 0, PM_REMOVE))
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

HWND RendeWindow::GetHWND() const
{
    return this->mHandle;
}


LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
    {
        DestroyWindow(hwnd);
        return 0;
    }
    default:
    {
        //retrieve ptr to window class
        WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        //forward message to window class handler
        return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_NCCREATE:
    {
        OutputDebugStringA("The window was created. \n");

        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
        if (pWindow == nullptr)
        {
            ErrorLogger::Log("critical Error: Pointer to window container is null during WM_NCCREATE.");
            exit(-1);
        }

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
        return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void RendeWindow::RegisterWindowClass()
{
    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = HandleMessageSetup;
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