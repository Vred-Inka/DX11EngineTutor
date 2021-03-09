#pragma once
#include "AdaptorReader.h"
#include "Shaders.h"
#include "Vertex.h"

#include <Effects.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "Effects11d.lib")
#include "d3dx11effect.h"

using namespace DirectX;


long double const pi = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899863;

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
    Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer2;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;




    //
    void OnResize();
    void UpdateScene(float dt);
    void DrawScene();

    void OnMouseDown(WPARAM bthState, int x, int y);
    void OnMouseUp(WPARAM bthState, int x, int y);
    void OnMouseMove(WPARAM bthState, int x, int y);

private:
    void BuildGeometryBuffers();
    void BuildFX();
    void BuildVertexLayout();

    Microsoft::WRL::ComPtr<ID3D11Buffer> mBoxVB;
    Microsoft::WRL::ComPtr<ID3D11Buffer> mBoxIB;
    
    Microsoft::WRL::ComPtr<ID3DX11Effect> mFX;
    Microsoft::WRL::ComPtr<ID3DX11EffectTechnique> mTech;
    Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> mfxWorldViewProj;
       
    Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;


    XMFLOAT4X4 mWorld;
    XMFLOAT4X4 mView;
    XMFLOAT4X4 mProj;

    float mTheta;
    float mPhi;
    float mRadius;
};

//  
//  