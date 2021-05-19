#pragma once
#include "AdaptorReader.h"
#include "Shaders.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "Camera.h"
#include "../Timer.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include "RenderableGameObject.h"
#include "Light.h"

class Graphics 
{
public:
    bool Initialize(HWND hwnd, int width, int height);
    void RenderFrame();
    void RenderImGuiFrame(float (&transtalionOffeset)[3], float& alpha);
    void DrawTextExemple();

    Camera mCamera;
    RenderableGameObject mGameObject;
    Light mLight;

private:
    bool InitializeDirectX(HWND hwnd);
    bool InitializeShaders();
    bool InitializeScene();
    void SetupImGui(HWND hwnd);

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
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;

    VertexShader mVertexShader;
    PixelShader mPixelShader;
    PixelShader mPixelShader_nolight;
    ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
    ConstantBuffer<CB_PS_light> cb_ps_light;

    ConstantBuffer<CB_VS_vertexshader> mConstantBuffer;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState_CullFront;

    Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;

    std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
    std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mGrassTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mBrickTexture;


    int mWindowWidth = 0;
    int mWindowHeight = 0;

    Timer mFPSTimer;
};