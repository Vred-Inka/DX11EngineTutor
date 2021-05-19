#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
    HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->mShaderBuffer.GetAddressOf());
    if (FAILED(hr))
    {
        std::wstring errorMsg = L"Failed to load structure: ";
        errorMsg += shaderpath;
        ErrorLogger::Log(hr, errorMsg);
        return false;
    }

    hr = device->CreateVertexShader(this->mShaderBuffer.Get()->GetBufferPointer(), this->mShaderBuffer->GetBufferSize(),
        NULL, this->mShader.GetAddressOf());
    if (FAILED(hr))
    {
        std::wstring errorMsg = L"Failed to create vertex shader: ";
        errorMsg += shaderpath;
        ErrorLogger::Log(hr, errorMsg);
        return false;
    }

    hr = device->CreateInputLayout(layoutDesc, numElements, mShaderBuffer->GetBufferPointer(),
        mShaderBuffer->GetBufferSize(), this->mInputLayout.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create input layout");
        return false;
    }

    return true;
}

ID3D11VertexShader * VertexShader::GetShader()
{
    return this->mShader.Get();
}

ID3D10Blob * VertexShader::GetBuffer()
{
    return this->mShaderBuffer.Get();
}

ID3D11InputLayout * VertexShader::GetInputLayout()
{
    return this->mInputLayout.Get();
}

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath)
{
    HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->mShaderBuffer.GetAddressOf());
    if (FAILED(hr))
    {
        std::wstring errorMsg = L"Failed to load sahder: ";
        errorMsg += shaderpath;
        ErrorLogger::Log(hr, errorMsg);
        return false;
    }

    hr = device->CreatePixelShader(this->mShaderBuffer.Get()->GetBufferPointer(), this->mShaderBuffer->GetBufferSize(),
        NULL, this->mShader.GetAddressOf());
    if (FAILED(hr))
    {
        std::wstring errorMsg = L"Failed to create vertex shader: ";
        errorMsg += shaderpath;
        ErrorLogger::Log(hr, errorMsg);
        return false;
    }

    return true;
}

ID3D11PixelShader * PixelShader::GetShader()
{
    return this->mShader.Get();
}

ID3D10Blob * PixelShader::GetBuffer()
{
    return this->mShaderBuffer.Get();
}
