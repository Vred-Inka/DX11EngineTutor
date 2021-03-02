#pragma once
#include "ErrorLogger.h"

class WindowContainer;

class RendeWindow
{
public:
    bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int with, int height);
    bool ProcessMessages();
    HWND GetHWND() const;
    ~RendeWindow();

private:
    void RegisterWindowClass();
    HWND mHandle = NULL; //Handle to this window
    HINSTANCE mhInstance = NULL; //Handle to Application instance

    std::string mWindowTitle = "";
    std::wstring mWindowTitleWide = L""; 
    std::string mWindowClass = "";
    std::wstring mWindowClassWide = L"";

    int mWidth = 0;
    int mHeight = 0;
};