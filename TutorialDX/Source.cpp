#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to call CoInitialize");
        return -1;
    }

    Engine engine;
    engine.Initialize(hInstance, "Title", "TestWindowClass", 1600, 900);

    while (engine.ProcessMessages() == true)
    {
        engine.Update();
        engine.RenderFrame();
    }

    return 0;
}