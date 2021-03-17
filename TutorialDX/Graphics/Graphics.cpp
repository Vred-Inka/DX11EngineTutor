#include "Graphics.h"
using namespace DirectX;

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
    this->mWindowHeight = height;
    this->mWindowWidth = width;

    if (!InitializeDirectX(hwnd))
    {
        return false;
    }

    if (!InitializeShaders())
    {
        return false;
    }

    if (!InitializeScene())
    {
        return false;
    }

    return true;
}

void Graphics::RenderFrame()
{
    float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    this->mDeviceConext->ClearRenderTargetView(this->mRenderTargetView.Get(), bgcolor);
    this->mDeviceConext->ClearDepthStencilView(this->mDepthStencilView.Get(), D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
   
    this->mDeviceConext->IASetInputLayout(this->mVertexShader.GetInputLayout());
    this->mDeviceConext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    this->mDeviceConext->RSSetState(this->mRasterizerState.Get());
    this->mDeviceConext->OMSetDepthStencilState(this->mDepthStencilState.Get(), 0);
    this->mDeviceConext->PSSetSamplers(0, 1, this->mSamplerState.GetAddressOf());
    this->mDeviceConext->VSSetShader(mVertexShader.GetShader(), NULL, 0);
    this->mDeviceConext->PSSetShader(mPixelShader.GetShader(), NULL, 0);

    UINT offset = 0;
    DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
    //Update Constant Buffer
    static DirectX::XMVECTOR eyePos = DirectX::XMVectorSet(0.0f, 4.0f, -2.0f, 0.0f);
    DirectX::XMFLOAT3 eyePosFlot3;
    DirectX::XMStoreFloat3(&eyePosFlot3, eyePos);
    eyePosFlot3.y += 0.01f;
    eyePos = DirectX::XMLoadFloat3(&eyePosFlot3);

    static DirectX::XMVECTOR lookAtPos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    static DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eyePos, lookAtPos, upVector);

    float fovDegrees = 90.0f;
    float fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
    float aspectRatio = static_cast<float>(this->mWindowWidth) / static_cast<float>(this->mWindowHeight);
    float nearZ = 0.1f;
    float farZ = 1000.0f;
    DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);


    mConstantBuffer.data.mat = world * viewMatrix * projectionMatrix;
    mConstantBuffer.data.mat = DirectX::XMMatrixTranspose(mConstantBuffer.data.mat);

    if (!mConstantBuffer.ApplyChanges())
    {
        return;
    }

    this->mDeviceConext->VSSetConstantBuffers(0, 1, this->mConstantBuffer.GetAddressOf());

    this->mDeviceConext->PSSetShaderResources(0, 1, this->mTexture.GetAddressOf());
    this->mDeviceConext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.StridePtr(), &offset);
    this->mDeviceConext->IASetIndexBuffer(mIndicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    this->mDeviceConext->DrawIndexed(mIndicesBuffer.BufferSize(), 0, 0);

    //draw text
    mSpriteBatch->Begin();
    mSpriteFont->DrawString(mSpriteBatch.get(), L"Test strinG", XMFLOAT2(0,0), Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));
    mSpriteBatch->End();
    
    this->mSwapChain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd)
{    
    if (!CreateSwapchain(hwnd))
    {
        return false;
    }
    
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = this->mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "GetBuffer failed.");
        return false;
    }

    hr = this->mDevice->CreateRenderTargetView(backBuffer.Get(), NULL, this->mRenderTargetView.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create render target view.");
        return false;
    }

    if (!CreateDepthStencil())
    {
        return false;
    }
    
    this->mDeviceConext->OMSetRenderTargets(1, this->mRenderTargetView.GetAddressOf(), this->mDepthStencilView.Get());

    if (!CreateDepthStencilState())
    {
        return false;
    }

    if (!CreateViewPort())
    {
        return false;
    }

    if (!CreateRasterizerState())
    {
        return false;
    }    

    InitializeFont();

    if (!CreateSampleState())
    {
        return false;
    }
 
    return true;
}

bool Graphics::InitializeShaders()
{
    std::wstring shaderFolder = L"";
#pragma region DetermineShaderPath
    if (IsDebuggerPresent() == TRUE)
    {
#ifdef _DEBUG
    #ifdef _WIN64
        shaderFolder = L"..\\x64\\Debug\\";
    #else
        shaderFolder = L"..\\Debug\\";
    #endif 
#else
    #ifdef _WIN64
        shaderFolder = L"..\\x64\\Release\\";
    #else
        shaderFolder = L"..\\Release\\"
    #endif
#endif
    }
    
    D3D11_INPUT_ELEMENT_DESC layout[] =
    { 
        {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    UINT numElements = ARRAYSIZE(layout);

    if (!mVertexShader.Initialize(this->mDevice, shaderFolder + L"vertexshader.cso", layout, numElements))
    {
        return false;
    }

    if (!mPixelShader.Initialize(this->mDevice, shaderFolder + L"pixelshader.cso", layout, numElements))
    {
        return false;
    }

    return true;
}

bool Graphics::InitializeScene()
{    
    if (!CreateVertexBuffer())
    {
        return false;
    }

    if (!CreateIndexesBuffer())
    {
        return false;
    }

    if (!CreateConstantBuffer())
    {
        return false;
    }

    if (!CreateTexture())
    {
        return false;
    }

    return true;
}

bool Graphics::CreateVertexBuffer()
{
    Vertex v[] =
    {
        Vertex(-0.5f, -0.5f, 0.0f,  0.0f, 1.0f),
        Vertex(-0.5f,  0.5f, 0.0f,  0.0f, 0.0f),
        Vertex(0.5f,  0.5f, 0.0f,  1.0f, 0.0f),
        Vertex(0.5f, -0.5f, 0.0f,  1.0f, 1.0f),
    };

    HRESULT hr = this->mVertexBuffer.Initialize(this->mDevice.Get(), v, ARRAYSIZE(v));
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create vertex buffer.");
        return false;
    }

    return true;
}

bool Graphics::CreateIndexesBuffer()
{
    DWORD indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    HRESULT hr = this->mIndicesBuffer.Initialize(this->mDevice.Get(), indices, ARRAYSIZE(indices));
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create indices buffer.");
        return false;
    }

    return true;
}

bool Graphics::CreateConstantBuffer()
{
    HRESULT hr = mConstantBuffer.Initialize(this->mDevice.Get(), this->mDeviceConext.Get());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to initialize constant buffer.");
        return false;
    }
    
    return true;
}

bool Graphics::CreateTexture()
{
    HRESULT hr = CreateWICTextureFromFile(this->mDevice.Get(), L"Data\\Textures\\tex1.png", nullptr, mTexture.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create vertex buffer.");
        return false;
    }

    return true;
}

bool Graphics::CreateSwapchain(HWND hwnd)
{
    std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

    if (adapters.size() < 1)
    {
        ErrorLogger::Log("No DXGI Adapters found.");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferDesc.Width = this->mWindowWidth;
    scd.BufferDesc.Height = this->mWindowHeight;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;

    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 1;
    scd.OutputWindow = hwnd;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        adapters[0].pAdapter, //IDXGI adapter
        D3D_DRIVER_TYPE_UNKNOWN,
        NULL,//for software driver type
        NULL,//flags for runtime layers
        NULL,//feature levels array
        0, //# of feature levels in array
        D3D11_SDK_VERSION,
        &scd, //swapchain description
        this->mSwapChain.GetAddressOf(),
        this->mDevice.GetAddressOf(),
        NULL, //supported feature level
        this->mDeviceConext.GetAddressOf()
    );

    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create device and swapchain.");
        return false;
    }

    return true;
}

bool Graphics::CreateDepthStencil()
{
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = this->mWindowWidth;
    depthStencilDesc.Height = this->mWindowHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    HRESULT hr = this->mDevice->CreateTexture2D(&depthStencilDesc, NULL, this->mDepthStencilBuffer.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create depth stencil buffer.");
        return false;
    }

    hr = this->mDevice->CreateDepthStencilView(this->mDepthStencilBuffer.Get(), NULL, this->mDepthStencilView.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create depth stencil view.");
        return false;
    }

    return true;
}

bool Graphics::CreateDepthStencilState()
{
    //create depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthStencilD;
    ZeroMemory(&depthStencilD, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthStencilD.DepthEnable = true;
    depthStencilD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilD.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    HRESULT hr = this->mDevice->CreateDepthStencilState(&depthStencilD, this->mDepthStencilState.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create depth Sencil state.");
        return false;
    }

    return true;
}

bool Graphics::CreateViewPort()
{
    //Create view port  
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = this->mWindowWidth;
    viewport.Height = this->mWindowHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    //Set the vieport
    this->mDeviceConext->RSSetViewports(1, &viewport);

    return true;
}

bool Graphics::CreateRasterizerState()
{
    //Create Rasterizer State
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = FALSE;

    HRESULT hr = this->mDevice->CreateRasterizerState(&rasterizerDesc, this->mRasterizerState.GetAddressOf());

    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create rasterizer state.");
        return false;
    }

    return true;
}

bool Graphics::InitializeFont()
{
    mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(this->mDeviceConext.Get());
    mSpriteFont = std::make_unique<DirectX::SpriteFont>(this->mDevice.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

    return true;
}

bool Graphics::CreateSampleState()
{
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT hr = this->mDevice->CreateSamplerState(&sampDesc, this->mSamplerState.GetAddressOf());

    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create sampler state.");
        return false;
    }

    return true;
}
