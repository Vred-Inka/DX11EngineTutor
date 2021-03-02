#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath)
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
