#pragma once
#include "Mesh.h"

using namespace DirectX;

class Model
{
public:
    bool Initialize(const std::string& filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader> & cb_vs_vertexshader);
    void Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewProjectionMatrix);

    const XMVECTOR& GetForwardVector();
    const XMVECTOR& GetRightVector();
    const XMVECTOR& GetLeftVector();
    const XMVECTOR& GetBackwardVector();

private:
    std::vector<Mesh> mMeshes;

    bool LoadModel(const std::string& filePath);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMaterial, unsigned int i, aiTextureType textureType);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
    int GetTextureIndex(aiString* pStr);

    ID3D11Device * mDevice = nullptr;
    ID3D11DeviceContext * mDeviceContext = nullptr;
    ConstantBuffer<CB_VS_vertexshader> * cb_vs_vertexshader = nullptr;
    std::string mDirectory = "";
};