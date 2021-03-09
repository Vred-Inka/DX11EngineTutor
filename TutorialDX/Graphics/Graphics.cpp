#include "Graphics.h"
#include <d3d11.h>


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

    if (!InitializeScene())
    {
        return false;
    }

    //mp
    XMMATRIX I = XMMatrixIdentity();
    XMStoreFloat4x4(&mWorld, I);
    XMStoreFloat4x4(&mView, I);
    XMStoreFloat4x4(&mProj, I);

    mBoxVB = nullptr;
    mBoxIB = nullptr;
    mFX = nullptr;
    mTech = nullptr;

    mTheta = 1.5f * pi;
    mPhi = 0.25f * pi;
    mRadius = 5.0f;

    BuildGeometryBuffers();
    BuildFX();
    BuildVertexLayout();


    return true;
}

void Graphics::RenderFrame()
{
    float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    this->mDeviceConext->ClearRenderTargetView(this->mRenderTargetView.Get(), bgcolor);
    this->mDeviceConext->ClearDepthStencilView(this->mDepthStencilView.Get(), D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
   
    this->mDeviceConext->IASetInputLayout(this->mVertexShader.GetInputLayout());
    this->mDeviceConext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    this->mDeviceConext->RSSetState(this->mRasterizerState.Get());
    this->mDeviceConext->OMSetDepthStencilState(this->mDepthStencilState.Get(), 0);
    this->mDeviceConext->VSSetShader(mVertexShader.GetShader(), NULL, 0);
    this->mDeviceConext->PSSetShader(mPixelShader.GetShader(), NULL, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    this->mDeviceConext->IASetVertexBuffers(0, 1, mVertexBuffer2.GetAddressOf(), &stride, &offset);
    this->mDeviceConext->Draw(3, 0);

    this->mDeviceConext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
    this->mDeviceConext->Draw(3, 0);

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

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = width;
    depthStencilDesc.Height = height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    hr = this->mDevice->CreateTexture2D(&depthStencilDesc, NULL, this->mDepthStencilBuffer.GetAddressOf());
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

    this->mDeviceConext->OMSetRenderTargets(1, this->mRenderTargetView.GetAddressOf(), this->mDepthStencilView.Get());

    //create depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthStencilD;
    ZeroMemory(&depthStencilD, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthStencilD.DepthEnable = true;
    depthStencilD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilD.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    hr = this->mDevice->CreateDepthStencilState(&depthStencilD, this->mDepthStencilState.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create depth Sencil state.");
        return false;
    }


    //Create view port  
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    //Set the vieport
    this->mDeviceConext->RSSetViewports(1, &viewport);

    //Create Rasterizer State
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    
    hr = this->mDevice->CreateRasterizerState(&rasterizerDesc, this->mRasterizerState.GetAddressOf());

    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create rasterizer state.");
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
        {"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
    Vertex v[] =
    {
        Vertex(-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f),
        Vertex(0.0f,   0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f),
        Vertex(0.5f,  -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f),
    };

    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v);
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    ZeroMemory(&vertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
    vertexBufferData.pSysMem = v;

    HRESULT hr = this->mDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->mVertexBuffer.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create vertex buffer.");       
        return false;
    }


    Vertex v2[] =
    {
        Vertex(-.25f, -0.25f, 0.5f,  1.0f, 1.0f, 0.0f, 1.0f),
        Vertex(0.25f,  0.25f, 0.5f,  1.0f, 1.0f, 0.0f, 1.0f),
        Vertex(0.25f, -0.25f, 0.5f,  1.0f, 1.0f, 0.0f, 1.0f),
    };
    
    D3D11_BUFFER_DESC vertexBufferDesc2;
    ZeroMemory(&vertexBufferDesc2, sizeof(D3D11_BUFFER_DESC));

    vertexBufferDesc2.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc2.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v2);
    vertexBufferDesc2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc2.CPUAccessFlags = 0;
    vertexBufferDesc2.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData2;
    ZeroMemory(&vertexBufferData2, sizeof(D3D11_SUBRESOURCE_DATA));
    vertexBufferData2.pSysMem = v2;

    hr = this->mDevice->CreateBuffer(&vertexBufferDesc2, &vertexBufferData2, this->mVertexBuffer2.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create vertex buffer.");
        return false;
    }

    return true;
}


///
//extra part
void Graphics::OnResize()
{

}

void Graphics::UpdateScene(float dt)
{
    float x = mRadius * sinf(mPhi) * cosf(mTheta);
    float z = mRadius * sinf(mPhi) * sinf(mTheta);
    float y = mRadius * cosf(mPhi);

    //buuild the view matrix
    XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX V = XMMatrixLookToLH(pos, target, up);
    XMStoreFloat4x4(&mView, V);

}

void Graphics::DrawScene()
{

    float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    this->mDeviceConext->ClearRenderTargetView(this->mRenderTargetView.Get(), bgcolor);
    this->mDeviceConext->ClearDepthStencilView(this->mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    this->mDeviceConext->IASetInputLayout(this->mVertexShader.GetInputLayout());
    this->mDeviceConext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    this->mDeviceConext->IASetVertexBuffers(0, 1, this->mBoxVB.GetAddressOf(), &stride, &offset);
    this->mDeviceConext->IASetIndexBuffer(this->mBoxIB.Get(),DXGI_FORMAT_R32_UINT, 0);

    XMMATRIX world = XMLoadFloat4x4(&mWorld);
    XMMATRIX view = XMLoadFloat4x4(&mView);
    XMMATRIX proj = XMLoadFloat4x4(&mProj);
    XMMATRIX worldViewProj = world * view * proj;

    mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

    D3DX11_TECHNIQUE_DESC techDesc;
    mTech->GetDesc(&techDesc);

    for (UINT p = 0; p < techDesc.Passes; ++p)
    {
        mTech->GetPassByIndex(p)->Apply(0, this->mDeviceConext.Get());
        this->mDeviceConext.Get()->DrawIndexed(36, 0, 0);
    }
    this->mSwapChain->Present(0, 0);
    
}

void Graphics::OnMouseDown(WPARAM bthState, int x, int y)
{
}

void Graphics::OnMouseUp(WPARAM bthState, int x, int y)
{
}

void Graphics::OnMouseMove(WPARAM bthState, int x, int y)
{
}

namespace Colors
{
    DirectX::XMFLOAT4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
    DirectX::XMFLOAT4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
    DirectX::XMFLOAT4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
    DirectX::XMFLOAT4 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
    DirectX::XMFLOAT4 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
    DirectX::XMFLOAT4 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
    DirectX::XMFLOAT4 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
    DirectX::XMFLOAT4 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

    DirectX::XMFLOAT4 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
    XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}


void Graphics::BuildGeometryBuffers()
{
    // Create vertex buffer
    Vertex vertices[] =
    {
    { -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
    { +1.0f, +1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
    { +1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
    { -1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
    { -1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    { +1.0f, +1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { +1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f }
    };
    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * 8;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    vbd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HRESULT(this->mDevice->CreateBuffer(&vbd, &vinitData, &mBoxVB));
    // Create the index buffer
    UINT indices[] = {
        // front face
        0, 1, 2,
        0, 2, 3,
        // back face
        4, 6, 5,
        4, 7, 6,
        // left face
        4, 5, 1,
        4, 1, 0,
        // right face
        3, 2, 6,
        3, 6, 7,
        // top face
        1, 5, 6,
        1, 6, 2,
        // bottom face
        4, 0, 3,
        4, 3, 7
    };
    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * 36;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    ibd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HRESULT(this->mDevice->CreateBuffer(&ibd, &iinitData, &mBoxIB));
}

void Graphics::BuildFX()
{
    DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    shaderFlags |= D3D10_SHADER_DEBUG;
    shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
    ID3D10Blob* compiledShader = 0;
    ID3D10Blob* compilationMsgs = 0;
   // HRESULT hr = D3DX11CompileEffectFromFile(L"FX/color.fx", 0, 0, 0,
      //  "fx_5_0", shaderFlags,
      //  0, 0, &compiledShader, &compilationMsgs);

    // compilationMsgs can store errors or warnings.
    if (compilationMsgs != 0)
    {
        MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
        //ReleaseCOM(compilationMsgs);
    } //
    //Even if there are no compilationMsgs, check to make sure there
        // were no other errors.
       // if (FAILED(hr))
      //  {
      //      DXTrace(__FILE__, (DWORD)__LINE__, hr,
      //          L"D3DX11CompileFromFile", true);
     //   }
    HRESULT(D3DX11CreateEffectFromMemory(
            compiledShader->GetBufferPointer(),
            compiledShader->GetBufferSize(),
            0, this->mDevice.Get(), &mFX));

        // Done with compiled shader.
        //ReleaseCOM(compiledShader);
        mTech = mFX->GetTechniqueByName("ColorTech");
        mfxWorldViewProj = mFX->GetVariableByName(
            "gWorldViewProj")->AsMatrix();
}

void Graphics::BuildVertexLayout()
{

    D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
    {
        {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    //create the input layout
    D3DX11_PASS_DESC passDesc;
    mTech->GetPassByIndex(0)->GetDesc(&passDesc);
    HRESULT(this->mDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout));

}
