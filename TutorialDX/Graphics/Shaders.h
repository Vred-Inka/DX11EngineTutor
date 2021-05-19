#pragma once
#include "../ErrorLogger.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

class VertexShader
{
public:
    bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> &device, std::wstring shaders, D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements);
    ID3D11VertexShader* GetShader();
    ID3D10Blob* GetBuffer();
    ID3D11InputLayout* GetInputLayout();

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> mShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D10Blob> mShaderBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout = nullptr;
};

class PixelShader
{
public:
    bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath);
    ID3D11PixelShader* GetShader();
    ID3D10Blob* GetBuffer();

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> mShader;
    Microsoft::WRL::ComPtr<ID3D10Blob> mShaderBuffer;
};
