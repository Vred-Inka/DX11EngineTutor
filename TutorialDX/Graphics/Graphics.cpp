#include "Graphics.h"
using namespace DirectX;

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
    this->mWindowHeight = height;
    this->mWindowWidth = width;
    this->mFPSTimer.Start();

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
    //cb_ps_light.data.dynamicLightColor = mLight.mLightColor;
    //cb_ps_light.data.dynamicLightStrength = mLight.mLightStrength;
    cb_ps_light.data.dynamicLightPosition = mLights[0].GetPositionFloat3();
    cb_ps_light.data.mDynamicLightAttenuation_a = mLights[0].mAttenuation_a;
    cb_ps_light.data.mDynamicLightAttenuation_b = mLights[0].mAttenuation_b;
    cb_ps_light.data.mDynamicLightAttenuation_c = mLights[0].mAttenuation_c;


    //cb_ps_light.data.dirLightAmbient = XMFLOAT3(0.2f, 0.2f, 0.2f);
    //cb_ps_light.data.dirlightDiffuse = XMFLOAT3(0.5f, 0.5f, 0.5f);
    //cb_ps_light.data.dirLightSpectular = XMFLOAT3(0.5f, 0.5f, 0.5f);
    //cb_ps_light.data.dirLightDirection = XMFLOAT3(0.57735f, 0.57735f, 0.57735f);


    cb_ps_light.ApplyChanges();
    mDeviceContext->PSSetConstantBuffers(0, 1, cb_ps_light.GetAddressOf());

    float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), bgcolor);
    mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    
    mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mDeviceContext->RSSetState(mRasterizerState.Get());
    mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);
    mDeviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
    mDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
    
    //sprite mask
    mDeviceContext->OMSetDepthStencilState(mDepthStencilState_drawMask.Get(), 0);
    mDeviceContext->IASetInputLayout(mVertexShader_2d.GetInputLayout());
    mDeviceContext->PSSetShader(mPixelShader_2d_discard.GetShader(), NULL, 0);
    mDeviceContext->VSSetShader(mVertexShader_2d.GetShader(), NULL, 0);
    mSprite.Draw(mCamera2D.GetWorldMatrix() * mCamera2D.GetOrthoMatrix());
       
    mDeviceContext->VSSetShader(mVertexShader.GetShader(), NULL, 0);
    mDeviceContext->PSSetShader(mPixelShader.GetShader(), NULL, 0);
    mDeviceContext->IASetInputLayout(mVertexShader.GetInputLayout());
    mDeviceContext->OMSetDepthStencilState(mDepthStencilState_applyMask.Get(), 0);

    XMFLOAT3 cameraPos = mCamera3D.GetPositionFloat3();
    XMFLOAT3 cameraRot = mCamera3D.GetRotationFloat3();
    float cameraPosition[3] = { cameraPos.x, cameraPos.y, cameraPos.z };
    float cameraRotation[3] = { cameraRot.x, cameraRot.y, cameraRot.z };

    XMFLOAT3 lightPos = mLights[0].GetPositionFloat3();
    XMFLOAT3 lightRot = mLights[0].GetRotationFloat3();
    float lightPosition[3] = { lightPos.x, lightPos.y, lightPos.z };
    float lightRotation[3] = { lightRot.x, lightRot.y, lightRot.z };

    XMFLOAT3 goPos = mGameObject.GetPositionFloat3();
    XMFLOAT3 goRot = mGameObject.GetRotationFloat3();
    float goPosition[3] = { goPos.x, goPos.y, goPos.z };
    float goRotation[3] = { goRot.x, goRot.y, goRot.z };



    static int counter = 0;
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    //Create ImGui Test Window
    ImGui::Begin("Light Controls");
    ImGui::DragFloat3("Ambient Light Color", &cb_ps_light.data.ambientLightColor.x, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Ambient Light Strength", &cb_ps_light.data.ambientLightStrength, 0.01f, 0.0f, 1.0f);
    ImGui::Separator();
    ImGui::DragFloat3("Denamic Light Color", &cb_ps_light.data.dynamicLightColor.x, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Dinamic Light Strength", &cb_ps_light.data.dynamicLightStrength, 0.01f, 0.0f, 100.0f);
    ImGui::End();

    ImGui::Begin("Positions");

    ImGui::Text("Camera Controls");
    ImGui::DragFloat3("Camera position", cameraPosition, 0.01f, -1000.0f, 1000.0f);
    ImGui::DragFloat3("Camera rotation", cameraRotation, 0.01f, -10.0f, 10.0f);
    ImGui::Separator();

    ImGui::Text("GameObject position");
    ImGui::DragFloat3("GO position", goPosition, 0.01f, -1000.0f, 1000.0f);
    ImGui::DragFloat3("GO rotation", goRotation, 0.01f, -10.0f, 10.0f);
    ImGui::Separator();

    ImGui::Text("Light position");
    ImGui::DragFloat3("Light position", lightPosition, 0.01f, -1000.0f, 1000.0f);
    ImGui::DragFloat3("Light rotation", lightRotation, 0.01f, -10.0f, 10.0f);
    ImGui::End();

    {
        mCamera3D.SetPosition(cameraPosition[0], cameraPosition[1], cameraPosition[2]);
        mCamera3D.SetRotation(cameraRotation[0], cameraRotation[1], cameraRotation[2]);

        mGameObject.SetPosition(goPosition[0], goPosition[1], goPosition[2]);
        mGameObject.SetRotation(goRotation[0], goRotation[1], goRotation[2]);

        mLights[0].SetPosition(lightPosition[0], lightPosition[1], lightPosition[2]);
        mLights[0].SetRotation(lightRotation[0], lightRotation[1], lightRotation[2]);
               
       // mScene.Draw(mCamera3D.GetViewMatrix() * mCamera3D.GetProjectionMatrix());
        mGameObject.Draw(mCamera3D.GetViewMatrix() * mCamera3D.GetProjectionMatrix());

        mDeviceContext->PSSetShader(mPixelShader_nolight.GetShader(), NULL, 0);

        for (Light& light : mLights)
        {
            light.Draw(mCamera3D.GetViewMatrix() * mCamera3D.GetProjectionMatrix());
        }
    }


    mSky.Draw(mDeviceContext.Get(), mCamera3D);
    

    //mDeviceContext->IASetInputLayout(mSkyVertexShader.GetInputLayout());


    DrawTextExemple();
    
    //ImGui::Begin("Attenuation Controls");
    //ImGui::DragFloat("Dynamic light Attenuation a", &cb_ps_light.data.mDynamicLightAttenuation_a, 0.01f, 0.1f, 10.0f);
    //ImGui::DragFloat("Dynamic light Attenuation b", &cb_ps_light.data.mDynamicLightAttenuation_b, 0.01f, 0.0f, 10.0f);
    //ImGui::DragFloat("Dynamic light Attenuation c", &cb_ps_light.data.mDynamicLightAttenuation_c, 0.01f, 0.0f, 10.0f);
    //ImGui::End();

    //Assemble Together Draw Data
    ImGui::Render();
    //Render Draw Data
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    mSwapChain->Present(0, NULL);
}

void Graphics::DrawTextExemple()
{
    //draw text
    static int fpsCounter = 0;
    static std::string fpsString = "FPS: 0";
    fpsCounter += 1;
    if (mFPSTimer.GetMilisecondsElapsed() > 1000.0f)
    {
        fpsString = "FPS: " + std::to_string(fpsCounter);
        fpsCounter = 0;
        mFPSTimer.Restart();
    }
    mSpriteBatch->Begin();
    mSpriteFont->DrawString(mSpriteBatch.get(), StringHelper::StringToWide(fpsString).c_str(), XMFLOAT2(0, 0), Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));
    mSpriteBatch->End();

}

bool Graphics::InitializeDirectX(HWND hwnd)
{    
    try
    {
        std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

        if (adapters.size() < 1)
        {
            ErrorLogger::Log("No IDXGI Adapters found.");
            return false;
        }

        DXGI_SWAP_CHAIN_DESC scd = { 0 };

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

        HRESULT hr;
        hr = D3D11CreateDeviceAndSwapChain(adapters[0].pAdapter, //IDXGI Adapter
            D3D_DRIVER_TYPE_UNKNOWN,
            NULL, //FOR SOFTWARE DRIVER TYPE
            NULL, //FLAGS FOR RUNTIME LAYERS
            NULL, //FEATURE LEVELS ARRAY
            0, //# OF FEATURE LEVELS IN ARRAY
            D3D11_SDK_VERSION,
            &scd, //Swapchain description
            this->mSwapChain.GetAddressOf(), //Swapchain Address
            this->mDevice.GetAddressOf(), //Device Address
            NULL, //Supported feature level
            this->mDeviceContext.GetAddressOf()); //Device Context Address

        COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain.");

        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
        hr = this->mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
        COM_ERROR_IF_FAILED(hr, "GetBuffer Failed.");

        hr = this->mDevice->CreateRenderTargetView(backBuffer.Get(), NULL, this->mRenderTargetView.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create render target view.");

        //Describe our Depth/Stencil Buffer
        CD3D11_TEXTURE2D_DESC depthStencilTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, this->mWindowWidth, this->mWindowHeight);
        depthStencilTextureDesc.MipLevels = 1;
        depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        hr = this->mDevice->CreateTexture2D(&depthStencilTextureDesc, NULL, this->mDepthStencilBuffer.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

        hr = this->mDevice->CreateDepthStencilView(this->mDepthStencilBuffer.Get(), NULL, this->mDepthStencilView.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");

        this->mDeviceContext->OMSetRenderTargets(1, this->mRenderTargetView.GetAddressOf(), this->mDepthStencilView.Get());

        //Create depth stencil state
        CD3D11_DEPTH_STENCIL_DESC depthstencildesc(D3D11_DEFAULT);
        depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

        hr = this->mDevice->CreateDepthStencilState(&depthstencildesc, this->mDepthStencilState.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

        //Create depth stencil state
        CD3D11_DEPTH_STENCIL_DESC depthstencildesc_drawMask(D3D11_DEFAULT);
        depthstencildesc_drawMask.DepthEnable = FALSE;
        depthstencildesc_drawMask.StencilEnable = TRUE;
        depthstencildesc_drawMask.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
        depthstencildesc_drawMask.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
        depthstencildesc_drawMask.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
        depthstencildesc_drawMask.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

        depthstencildesc_drawMask.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
        depthstencildesc_drawMask.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
        depthstencildesc_drawMask.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
        depthstencildesc_drawMask.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT;

        hr = this->mDevice->CreateDepthStencilState(&depthstencildesc_drawMask, this->mDepthStencilState_drawMask.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state for drawing mask.");

        CD3D11_DEPTH_STENCIL_DESC depthstencildesc_applyMask(D3D11_DEFAULT);
        depthstencildesc_applyMask.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
        depthstencildesc_applyMask.StencilEnable = TRUE;
        depthstencildesc_applyMask.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
        depthstencildesc_applyMask.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
        depthstencildesc_applyMask.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
        depthstencildesc_applyMask.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

        depthstencildesc_applyMask.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
        depthstencildesc_applyMask.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
        depthstencildesc_applyMask.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
        depthstencildesc_applyMask.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

        hr = this->mDevice->CreateDepthStencilState(&depthstencildesc_applyMask, this->mDepthStencilState_applyMask.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state for applying mask.");


        //Create & set the Viewport
        CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(this->mWindowWidth), static_cast<float>(this->mWindowHeight));;
        this->mDeviceContext->RSSetViewports(1, &viewport);

        //Create Rasterizer State
        CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
        hr = this->mDevice->CreateRasterizerState(&rasterizerDesc, this->mRasterizerState.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

        //Create Rasterizer State for culling front
        CD3D11_RASTERIZER_DESC rasterizerDesc_CullFront(D3D11_DEFAULT);
        rasterizerDesc_CullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
        hr = this->mDevice->CreateRasterizerState(&rasterizerDesc_CullFront, this->mRasterizerState_CullFront.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

        //Create Blend State
        D3D11_RENDER_TARGET_BLEND_DESC rtbd = { 0 };
        rtbd.BlendEnable = true;
        rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
        rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
        rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
        rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
        rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
        rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
        rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

        D3D11_BLEND_DESC blendDesc = { 0 };
        blendDesc.RenderTarget[0] = rtbd;

        hr = this->mDevice->CreateBlendState(&blendDesc, this->mBlendState.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");

        mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(this->mDeviceContext.Get());
        mSpriteFont = std::make_unique<DirectX::SpriteFont>(this->mDevice.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

        //Create sampler description for sampler state
        CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        hr = this->mDevice->CreateSamplerState(&sampDesc, this->mSamplerState.GetAddressOf()); //Create sampler state
        COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");
    }
    catch (COMException & exception)
    {
        ErrorLogger::Log(exception);
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
    
    D3D11_INPUT_ELEMENT_DESC layout3D[] =
    { 
        {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };    
    
    D3D11_INPUT_ELEMENT_DESC layout2D[] =
    { 
        {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    UINT numElements3D = ARRAYSIZE(layout3D);

    UINT numElements2D = ARRAYSIZE(layout2D);

    if (!mVertexShader_2d.Initialize(this->mDevice, shaderFolder + L"vertexshader_2d.cso", layout2D, numElements2D))
    {
        return false;
    }

    if (!mVertexShader.Initialize(this->mDevice, shaderFolder + L"vertexshader.cso", layout3D, numElements3D))
    {
        return false;
    }

    if (!mPixelShader_2d.Initialize(this->mDevice, shaderFolder + L"pixelshader_2d.cso"))
    {
        return false;
    }

    if (!mPixelShader_2d_discard.Initialize(this->mDevice, shaderFolder + L"pixelshader_2d_discard.cso"))
    {
        return false;
    }

    if (!mPixelShader.Initialize(this->mDevice, shaderFolder + L"pixelshader.cso"))
    {
        return false;
    }
       
    if (!mPixelShader_nolight.Initialize(this->mDevice, shaderFolder + L"pixelshader_nolight.cso"))
    {
        return false;
    }
   
    return true;
}

bool Graphics::InitializeScene()
{    
    try
    {
        //Load Texture
        HRESULT hr = DirectX::CreateWICTextureFromFile(this->mDevice.Get(), L"Data\\Textures\\t1.jpg", nullptr, mGrassTexture.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

        hr = DirectX::CreateWICTextureFromFile(this->mDevice.Get(), L"Data\\Textures\\t2.jpg", nullptr, mTexture.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

        hr = DirectX::CreateWICTextureFromFile(this->mDevice.Get(), L"Data\\Textures\\t6.jpg", nullptr, mCubeTexture.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

        //Initialize Constant Buffer(s)
        hr = this->cb_vs_vertexshader_2d.Initialize(this->mDevice.Get(), this->mDeviceContext.Get());
        COM_ERROR_IF_FAILED(hr, "Failed to initialize 2d constant buffer.");     

        hr = this->cb_vs_vertexshader.Initialize(this->mDevice.Get(), this->mDeviceContext.Get());
        COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

        hr = this->cb_ps_light.Initialize(this->mDevice.Get(), this->mDeviceContext.Get());
        COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

        this->cb_ps_light.data.ambientLightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
        this->cb_ps_light.data.ambientLightStrength = 1.0f;


        if (!mScene.Initialize(
            "Data\\Scenes\\Scene1\\Space_Station_Scene.obj",
            //"Data\\Scenes\\Coffee\\coffee.obj",
            //"Data\\Scenes\\room.fbx",
            //"Data\\Scenes\\full_1.fbx",
            //"Data\\Scenes\\Castle\\Castle OBJ.obj",
            //"Data/Objects/light.fbx",
            this->mDevice.Get(), this->mDeviceContext.Get(), this->cb_vs_vertexshader))
        {
            return false;
        }

        //mScene.SetPosition(0.0f, 0.25f, 0.0f);


        if (!mGameObject.Initialize(
            //"Data\\Objects\\samp\\blue_cube_notexture.fbx",
            //"Data\\Objects\\fbx\\Dragon.fbx",
            "Data\\Objects\\nanosuit\\nanosuit.obj",
            //"Data\\Objects\\free\\spot\\spot.obj",
            //"Data\\Objects\\Samples\\orange_embeddedtexture.fbx",
            //"Data\\Objects\\Samples\\orange_disktexture.fbx",
            //"Data\\Objects\\Samples\\person_embeddedindexed.blend",
            //"Data\\Objects\\Samples\\dodge_challenger.fbx",
            //"Data\\Objects\\Samples\\Palm_Tree\\10446_Palm_Tree_v1_max2010_iteration-2.obj",
            this->mDevice.Get(), this->mDeviceContext.Get(), this->cb_vs_vertexshader))
        {
            return false;
        }

        mGameObject.SetPosition(10.0f, 0.0f, -12.0f);

        for (int i = 0; i < 5; i++)
        {
            Light pointLight;
            pointLight.mLightColor = XMFLOAT3(i*0.1, 1.0f - i * 0.1f, 0.2f* i);
            mLights.push_back(pointLight);
            if (!mLights[i].Initialize(this->mDevice.Get(), this->mDeviceContext.Get(), this->cb_vs_vertexshader))
                return false;            
            mLights[i].SetPosition(5*i + 5.0f,  7.0f, 10*i + -15.0f);
        }    

        cb_ps_light.data.gDirLight.Ambient = mLights[0].mAttenuation_c;
        cb_ps_light.data.mDynamicLightAttenuation_c = mLights[0].mAttenuation_c;
        cb_ps_light.data.mDynamicLightAttenuation_c = mLights[0].mAttenuation_c;

        mDirLight.Ambient = cb_ps_light.data.gDirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
        mDirLight.Diffuse = cb_ps_light.data.gDirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
        mDirLight.Specular = cb_ps_light.data.gDirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
        mDirLight.Direction = cb_ps_light.data.gDirLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
        // Point light--position is changed every frame to animate
        // in UpdateScene function.
        mPointLight.Ambient = cb_ps_light.data.gPointLight.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
        mPointLight.Diffuse = cb_ps_light.data.gPointLight.Diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
        mPointLight.Specular = cb_ps_light.data.gPointLight.Specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
        mPointLight.Att = cb_ps_light.data.gPointLight.Att = XMFLOAT3(0.0f, 0.1f, 0.0f);
        mPointLight.Range = cb_ps_light.data.gPointLight.Range = 25.0f;
        // Spot light--position and direction changed every frame to
        // animate in UpdateScene function.
        mSpotLight.Ambient = cb_ps_light.data.gSpotLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
        mSpotLight.Diffuse = cb_ps_light.data.gSpotLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
        mSpotLight.Specular =  cb_ps_light.data.gSpotLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        mSpotLight.Att = cb_ps_light.data.gSpotLight.Att = XMFLOAT3(1.0f, 0.0f, 0.0f);
        mSpotLight.Spot = cb_ps_light.data.gSpotLight.Spot = 96.0f;
        mSpotLight.Range = cb_ps_light.data.gSpotLight.Range = 10000.0f;

        mLandMat.Ambient =  XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
        mLandMat.Diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
        mLandMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);
        mWavesMat.Ambient = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
        mWavesMat.Diffuse = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
        mWavesMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);

        cb_ps_light.data.dynamicLightStrength = 500.0f;
        cb_ps_light.data.ambientLightStrength = 0.5f;
        cb_ps_light.data.dynamicLightColor.x  = 1.0f;
        mLights[0].SetPosition(5.0f, 12.0f, -15.0f);



       if (!mSky.Initialize(mDevice.Get(), mDeviceContext.Get(), 5000.0f))
           return false;

        
        if (!mSprite.Initialize(mDevice.Get(), mDeviceContext.Get(), 256, 256, "Data/Textures/mask_2.png", cb_vs_vertexshader_2d))
            return false;

        mSprite.SetScale(mWindowWidth, mWindowHeight, 0.0f);
        mSprite.SetPosition(XMFLOAT3(mWindowWidth/2 - mSprite.GetWidth()/2, mWindowHeight/2 -mSprite.GetHeight()/2, 0.0f ));
        
        mCamera2D.SetProjectionValues(mWindowWidth, mWindowHeight, 0.0f, 1.0f);

        mCamera3D.SetPosition(5.0f, 12.0f, -36.0f);
        mCamera3D.SetRotation(0.2f, 0.7f, 0.0f);

        mCamera3D.SetProjectionValues(90.0f, static_cast<float>(mWindowWidth) / static_cast<float>(mWindowHeight), 0.1f, 10000.0f);
    }
    catch (COMException & exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }

    return true;
}

void Graphics::SetupImGui(HWND hwnd)
{
    //Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(this->mDevice.Get(), this->mDeviceContext.Get());
    ImGui::StyleColorsDark();
}

bool Graphics::CreateVertexBuffer()
{
    //in a model.h
    return true;
}

bool Graphics::CreateIndexesBuffer()
{
    // in a model.h
    return true;
}

bool Graphics::CreateConstantBuffer()
{
    try
    {
        HRESULT hr = cb_vs_vertexshader.Initialize(this->mDevice.Get(), this->mDeviceContext.Get());
        COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

         hr = cb_ps_light.Initialize(this->mDevice.Get(), this->mDeviceContext.Get());
        COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

        this->cb_ps_light.data.ambientLightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
        this->cb_ps_light.data.ambientLightStrength = 1.0f;
        


    }
    catch (COMException& exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }
    
    return true;
}

bool Graphics::CreateTexture()
{
    try
    {
        std::wstring flagTexture = L"Data\\Textures\\t1.jpg";
        std::wstring wolfTexture = L"Data\\Objects\\wolf\\twxtures\\Wolf_Body.jpg";
        HRESULT hr = CreateWICTextureFromFile(this->mDevice.Get(), L"Data\\Objects\\free\\spot\\spot\\spot_texture.png", nullptr, mTexture.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

        hr = CreateWICTextureFromFile(this->mDevice.Get(), L"Data\\Textures\\t4.jpg", nullptr, mGrassTexture.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

        hr = CreateWICTextureFromFile(this->mDevice.Get(), L"Data\\Textures\\t6.jpg", nullptr, mCubeTexture.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");
    }
    catch (COMException& exception)
    {
        ErrorLogger::Log(exception);
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

    DXGI_SWAP_CHAIN_DESC scd = {0};
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
        this->mDeviceContext.GetAddressOf()
    );

    //COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain.");

    return true;
}

bool Graphics::CreateDepthStencil()
{
    try
    {
        CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, mWindowWidth, mWindowHeight);
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        HRESULT hr = this->mDevice->CreateTexture2D(&depthStencilDesc, NULL, this->mDepthStencilBuffer.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

        hr = this->mDevice->CreateDepthStencilView(this->mDepthStencilBuffer.Get(), NULL, this->mDepthStencilView.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");
    }
    catch (COMException& exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }

    return true;
}

bool Graphics::CreateDepthStencilState()
{
    try
    {
        //create depth stencil state
        CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

        HRESULT hr = this->mDevice->CreateDepthStencilState(&depthStencilDesc, this->mDepthStencilState.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create depth Sencil state.");
    }
    catch (COMException& exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }

    return true;
}

bool Graphics::CreateViewPort()
{
    //Create view port  
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(mWindowWidth), static_cast<float>(mWindowHeight));
    this->mDeviceContext->RSSetViewports(1, &viewport);

    return true;
}

bool Graphics::CreateRasterizerState()
{
    //Create Rasterizer State
    CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);

    HRESULT hr = this->mDevice->CreateRasterizerState(&rasterizerDesc, this->mRasterizerState.GetAddressOf());

    COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

    //Create Rasterizer State for culling front
    CD3D11_RASTERIZER_DESC rasterizerDesc_CullFront(D3D11_DEFAULT);
   // rasterizerDesc_CullFront.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
    rasterizerDesc_CullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
   // rasterizerDesc_CullFront.FrontCounterClockwise = FALSE;

    hr = this->mDevice->CreateRasterizerState(&rasterizerDesc_CullFront, this->mRasterizerState_CullFront.GetAddressOf());

    COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

    //Create Blend State    
    D3D11_RENDER_TARGET_BLEND_DESC rtbd = {0};
    rtbd.BlendEnable = true;
    rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
    rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
    rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
    rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

    D3D11_BLEND_DESC blendDesc = { 0 };
    blendDesc.RenderTarget[0] = rtbd;

    hr = mDevice->CreateBlendState(&blendDesc, mBlendState.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");

    return true;
}

bool Graphics::InitializeFont()
{
    mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(this->mDeviceContext.Get());
    mSpriteFont = std::make_unique<DirectX::SpriteFont>(this->mDevice.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

    return true;
}

bool Graphics::CreateSampleState()
{
    CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    HRESULT hr = this->mDevice->CreateSamplerState(&sampDesc, this->mSamplerState.GetAddressOf());

    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create sampler state.");
        return false;
    }

    return true;
}
