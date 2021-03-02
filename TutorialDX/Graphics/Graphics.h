#pragma once
#include "AdaptorReader.h"
#include "Shaders.h"
#include "Vertex.h"

class Graphics 
{
public:
    bool Initialize(HWND hwnd, int width, int height);
    void RenderFrame();
private:
    bool InitializeDirectX(HWND hwnd, int width, int height);
    bool InitializeShaders();
    bool InitializeScene();

    Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceConext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;

    VertexShader mVertexShader;
    PixelShader mPixelShader;

    Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
};