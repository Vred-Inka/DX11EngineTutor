#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include "Color.h"
#include <assimp/material.h>

enum class TextureStorageType
{
    Invalid,
    None,
    EmbeddedIndexCompressed,
    EmbeddedIndexNonCompressed,
    EmbeddedCompressed,
    EmbeddedNonCompressed,
    Disk
};

class Texture
{
public:
    Texture(ID3D11Device* device, const Color& color, aiTextureType type);
    Texture(ID3D11Device* device, const Color* colorData, UINT width, UINT height, aiTextureType type);
    Texture(ID3D11Device* device, const std::string& filePath, aiTextureType type);
    Texture(ID3D11Device* device, const uint8_t* pData, size_t size, aiTextureType type);
    aiTextureType GetType();
    ID3D11ShaderResourceView* GetTextureResourceView();
    ID3D11ShaderResourceView** GetTextureResourceViewAdress();

private:
    void Initialize1x1Colortexture(ID3D11Device* device, const Color& colorData, aiTextureType type);
    void InitializeColortexture(ID3D11Device* device, const Color* colorData, UINT width, UINT height, aiTextureType type);
    Microsoft::WRL::ComPtr<ID3D11Resource> mTexture = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTextureView = nullptr;
    aiTextureType mType = aiTextureType::aiTextureType_REFLECTION;
    std::string mPath = "";
};