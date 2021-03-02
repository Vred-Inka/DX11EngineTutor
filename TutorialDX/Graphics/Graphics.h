#pragma once
#include "AdaptorReader.h"

class Graphics 
{
public:
    bool Initialize(HWND hwnd, int width, int height);
private:
    bool InitializeDirectX(HWND hwnd, int width, int height);

    Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceConent;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
};