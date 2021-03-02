#pragma once
#include "AdaptorReader.h"
#include "Shaders.h"

class Graphics 
{
public:
    bool Initialize(HWND hwnd, int width, int height);
    void RenderFrame();
private:
    bool InitializeDirectX(HWND hwnd, int width, int height);
    bool InitializeShaders();

    Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceConext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;

    VertexShader mVertexShader;
    PixelShader mPixelShader;
};