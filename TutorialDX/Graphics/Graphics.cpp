#include "Graphics.h"
using namespace DirectX;

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
    this->mWindowHeight = height;
    this->mWindowWidth = width;
    this->mFSTimer.Start();

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

    SetupImGui(hwnd);

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
    this->mDeviceConext->OMSetBlendState(mBlendState.Get(), NULL, 0xFFFFFFFF);
    this->mDeviceConext->PSSetSamplers(0, 1, this->mSamplerState.GetAddressOf());
    this->mDeviceConext->VSSetShader(mVertexShader.GetShader(), NULL, 0);
    this->mDeviceConext->PSSetShader(mPixelShader.GetShader(), NULL, 0);

    UINT offset = 0;
    static float translationOffset[3] = { 0, 0, -1.0f };
/*
    {//brick
        static float translationOffset[3] = { 0, 0, 4.0f };
        DirectX::XMMATRIX world = XMMatrixScaling(3.0f, 3.0f, 3.0f) *  XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
        cb_vs_vertexshader.data.mat = world * mCamera.GetViewMatrix() * mCamera.GetProjectionMatrix();
        cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexshader.data.mat);

        if (!cb_vs_vertexshader.ApplyChanges())
        {
            return;
        }

        this->mDeviceConext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());

        this->cb_ps_pixelshader.data.alpha = 1.0f;
        this->cb_ps_pixelshader.ApplyChanges();
        this->mDeviceConext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader.GetAddressOf());

        this->mDeviceConext->PSSetShaderResources(0, 1, this->mBrickTexture.GetAddressOf());
        this->mDeviceConext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.StridePtr(), &offset);
        this->mDeviceConext->IASetIndexBuffer(mIndicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        this->mDeviceConext->DrawIndexed(mIndicesBuffer.BufferSize(), 0, 0);
    }
    */
    static float alpha = 0.5f;

    {//pic       
        DirectX::XMMATRIX world = XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
        cb_vs_vertexshader.data.mat = world * mCamera.GetViewMatrix() * mCamera.GetProjectionMatrix();
        cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexshader.data.mat);

        if (!cb_vs_vertexshader.ApplyChanges())
        {
            return;
        }

        this->mDeviceConext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());


        this->cb_ps_pixelshader.data.alpha = alpha;
        this->cb_ps_pixelshader.ApplyChanges();
        this->mDeviceConext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader.GetAddressOf());

        this->mDeviceConext->PSSetShaderResources(0, 1, this->mBrickTexture.GetAddressOf());
        this->mDeviceConext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.StridePtr(), &offset);
        this->mDeviceConext->IASetIndexBuffer(mIndicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        this->mDeviceConext->RSSetState(this->mRasterizerState_CullFront.Get());
        this->mDeviceConext->DrawIndexed(mIndicesBuffer.BufferSize(), 0, 0);
        this->mDeviceConext->RSSetState(this->mRasterizerState.Get());
        this->mDeviceConext->DrawIndexed(mIndicesBuffer.BufferSize(), 0, 0);
    }

    {//pic       
        static float translationOffset[3] = { 0, 0, 5.0f };
        DirectX::XMMATRIX world = XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
        cb_vs_vertexshader.data.mat = world * mCamera.GetViewMatrix() * mCamera.GetProjectionMatrix();
        cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexshader.data.mat);

        if (!cb_vs_vertexshader.ApplyChanges())
        {
            return;
        }

        this->mDeviceConext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());


        this->cb_ps_pixelshader.data.alpha = 1.0f;
        this->cb_ps_pixelshader.ApplyChanges();
        this->mDeviceConext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader.GetAddressOf());

        this->mDeviceConext->PSSetShaderResources(0, 1, this->mGrassTexture.GetAddressOf());
        this->mDeviceConext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.StridePtr(), &offset);
        this->mDeviceConext->IASetIndexBuffer(mIndicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        this->mDeviceConext->DrawIndexed(mIndicesBuffer.BufferSize(), 0, 0);
    }

    /*
    {//Grass
        static float translationOffset[3] = { 0, 0, 0.0f };
        DirectX::XMMATRIX world = XMMatrixScaling(3.0f, 3.0f, 3.0f) *  XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
        cb_vs_vertexshader.data.mat = world * mCamera.GetViewMatrix() * mCamera.GetProjectionMatrix();
        cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexshader.data.mat);

        if (!cb_vs_vertexshader.ApplyChanges())
        {
            return;
        }

        this->mDeviceConext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());

        this->cb_ps_pixelshader.data.alpha = 1.0f;
        this->cb_ps_pixelshader.ApplyChanges();
        this->mDeviceConext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader.GetAddressOf());

        this->mDeviceConext->PSSetShaderResources(0, 1, this->mGrassTexture.GetAddressOf());
        this->mDeviceConext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.StridePtr(), &offset);
        this->mDeviceConext->IASetIndexBuffer(mIndicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        this->mDeviceConext->DrawIndexed(mIndicesBuffer.BufferSize(), 0, 0);
    }
    */

    
    DrawTextExemple();

    RenderImGuiFrame(translationOffset, alpha);
    
    this->mSwapChain->Present(0, NULL);
}

void Graphics::DrawTextExemple()
{
    //draw text
    static int fpsCounter = 0;
    static std::string fpsString = "FPS: 0";
    fpsCounter += 1;
    if (mFSTimer.GetMilisecondsElapsed() > 1000.0f)
    {
        fpsString = "FPS: " + std::to_string(fpsCounter);
        fpsCounter = 0;
        mFSTimer.Restart();
    }
    mSpriteBatch->Begin();
    mSpriteFont->DrawString(mSpriteBatch.get(), StringConverter::StringToWide(fpsString).c_str(), XMFLOAT2(0, 0), Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));
    mSpriteBatch->End();
}

void Graphics::RenderImGuiFrame(float(&transtalionOffeset)[3], float& alpha)
{
    static int counter = 0;
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    //create ImGui Test Window
    ImGui::Begin("Test");
    ImGui::Text("...");
   /* if (ImGui::Button("CLick Me"))
    {
        counter += 1;
    }
    ImGui::SameLine();
    std::string clickCount = "Click Count: " + std::to_string(counter);
    ImGui::Text(clickCount.c_str());
    */
    ImGui::DragFloat("Alpha", &alpha, 0.1f, 0.0f, 1.0f);
    ImGui::DragFloat3("Translation X/Y/Z", transtalionOffeset, 0.1f, -5.0f, 5.0f);
    ImGui::End();
    //Assemble Together Draw Data
    ImGui::Render();
    //Render Draw Data
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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
    
    mCamera.SetPosition(0.0f, 0.0f, -2.0f);
    mCamera.SetProjectionValues(90.0f, static_cast<float>(mWindowWidth) / static_cast<float>(mWindowHeight), 0.1f, 1000.0f);

    return true;
}

void Graphics::SetupImGui(HWND hwnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(this->mDevice.Get(), this->mDeviceConext.Get());
    ImGui::StyleColorsDark();
}

bool Graphics::CreateVertexBuffer()
{
    Vertex v[] =
    {
        //front part
        Vertex(-0.5f, -0.5f, -0.5f,  0.0f, 1.0f),
        Vertex(-0.5f,  0.5f, -0.5f,  0.0f, 0.0f),
        Vertex(0.5f,  0.5f, -0.5f,  1.0f, 0.0f),
        Vertex(0.5f, -0.5f, -0.5f,  1.0f, 1.0f),

        //back part
        Vertex(-0.5f, -0.5f, 0.5f,  0.0f, 1.0f),
        Vertex(-0.5f,  0.5f, 0.5f,  0.0f, 0.0f),
        Vertex(0.5f,  0.5f, 0.5f,  1.0f, 0.0f),
        Vertex(0.5f, -0.5f, 0.5f,  1.0f, 1.0f),
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
        //front
        0, 1, 2,
        0, 2, 3,
        //back
        4, 7, 6,
        4, 6, 5,
        //right
        3, 2, 6,
        3, 6, 7,
        //left
        4, 5, 1,
        4, 1, 0,
        //top
        1, 5, 6,
        1, 6, 2, 
        //bottom
        0, 3, 7,
        0, 7, 4
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
    HRESULT hr = cb_vs_vertexshader.Initialize(this->mDevice.Get(), this->mDeviceConext.Get());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to initialize constant buffer.");
        return false;
    }

    hr = cb_ps_pixelshader.Initialize(this->mDevice.Get(), this->mDeviceConext.Get());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to initialize constant buffer.");
        return false;
    }
    
    return true;
}

bool Graphics::CreateTexture()
{
    HRESULT hr = CreateWICTextureFromFile(this->mDevice.Get(), L"Data\\Textures\\t1.jpg", nullptr, mTexture.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create vertex buffer.");
        return false;
    }

    hr = CreateWICTextureFromFile(this->mDevice.Get(), L"Data\\Textures\\t4.jpg", nullptr, mGrassTexture.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create vertex buffer.");
        return false;
    }

    hr = CreateWICTextureFromFile(this->mDevice.Get(), L"Data\\Textures\\t6.jpg", nullptr, mBrickTexture.GetAddressOf());
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

    DXGI_SWAP_CHAIN_DESC scd {};
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

    //Create Rasterizer State for culling front
    D3D11_RASTERIZER_DESC rasterizerDesc_CullFront;
    ZeroMemory(&rasterizerDesc_CullFront, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc_CullFront.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc_CullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
    rasterizerDesc_CullFront.FrontCounterClockwise = FALSE;

    hr = this->mDevice->CreateRasterizerState(&rasterizerDesc_CullFront, this->mRasterizerState_CullFront.GetAddressOf());

    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create rasterizer state.");
        return false;
    }

    //Create Blend State
    D3D11_BLEND_DESC blendDesc = {};
    D3D11_RENDER_TARGET_BLEND_DESC rtbd = {};
    rtbd.BlendEnable = true;
    rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
    rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
    rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
    rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

    blendDesc.RenderTarget[0] = rtbd;

    hr = mDevice->CreateBlendState(&blendDesc, mBlendState.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create blend state.");
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
