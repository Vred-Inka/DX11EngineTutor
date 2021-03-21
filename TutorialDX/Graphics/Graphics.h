#pragma once
#include "AdaptorReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include "../Timer.h"

class Graphics 
{
public:
    bool Initialize(HWND hwnd, int width, int height);
    void RenderFrame();

    Camera mCamera;

private:
    bool InitializeDirectX(HWND hwnd);
    bool InitializeShaders();
    bool InitializeScene();
    bool CreateTexture();
    bool CreateVertexBuffer();
    bool CreateIndexesBuffer();
    bool CreateConstantBuffer();

    bool CreateSwapchain(HWND hwnd);
    bool CreateDepthStencil();
    bool CreateDepthStencilState();
    bool CreateViewPort();
    bool CreateRasterizerState();
    bool InitializeFont();

    bool CreateSampleState();

    Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceConext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;

    VertexShader mVertexShader;
    PixelShader mPixelShader;

    VertexBuffer<Vertex> mVertexBuffer;
    IndexBuffer mIndicesBuffer;
    ConstantBuffer<CB_VS_vertexshader> mConstantBuffer;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;

    std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
    std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;

    int mWindowWidth = 0;
    int mWindowHeight = 0;

    Timer mFSTimer;
};