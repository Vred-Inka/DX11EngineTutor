#include "Graphics.h"
bool Graphics::Initialize(HWND hwnd, int width, int height)
{
    if (!InitializeDirectX(hwnd, width, height))
    {
        return false;
    }

    if (!InitializeShaders())
    {
        return false;
    }

    return true;
}

void Graphics::RenderFrame()
{
    float bgcolor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    this->mDeviceConext->ClearRenderTargetView(this->mRenderTargetView.Get(), bgcolor);
    this->mSwapChain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
{
    std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

    if (adapters.size() < 1)
    {
        ErrorLogger::Log("No DXGI Adapters found.");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
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

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    hr = this->mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
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

    this->mDeviceConext->OMSetRenderTargets(1, this->mRenderTargetView.GetAddressOf(), NULL);

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = width;
    viewport.Height = height;

    this->mDeviceConext->RSSetViewports(1, &viewport);

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
        {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
