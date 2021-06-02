#include "Model.h"

bool Model::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
    this->mDevice = device;
    this->mDeviceContext = deviceContext;
    this->cb_vs_vertexshader = &cb_vs_vertexshader;

    try
    {
        if (!this->LoadModel(filePath))
        {
            return false;
        }
    }
    catch (COMException & exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }

    return true;
}

void Model::Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewProjectionMatrix)
{
    this->mDeviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());

    for (int i = 0; i < mMeshes.size(); i++)
    {
        this->cb_vs_vertexshader->data.wvpMatrix = mMeshes[i].GetTransformMatrix()  * worldMatrix * viewProjectionMatrix ; //Calculate World-View-Projection Matrix
        this->cb_vs_vertexshader->data.worldMatrix = mMeshes[i].GetTransformMatrix() * worldMatrix; 
        this->cb_vs_vertexshader->ApplyChanges();
        mMeshes[i].Draw();
    }
}

bool Model::LoadModel(const std::string & filePath)
{
    mDirectory = StringHelper::GetDirectoryFromPath(filePath);
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(filePath,
                                                aiProcess_Triangulate | 
                                               aiProcess_ConvertToLeftHanded);
    
    if (pScene == nullptr)
    {
        ErrorLogger::Log(importer.GetErrorString());
        return false;
    }

    this->ProcessNode(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());
    return true;
}

void Model::ProcessNode(aiNode* node, const aiScene * scene, const XMMATRIX& parentTransformMatrix)
{
    XMMATRIX nodeTransformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;

    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        mMeshes.push_back(this->ProcessMesh(mesh, scene, nodeTransformMatrix));
    }

    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        this->ProcessNode(node->mChildren[i], scene, nodeTransformMatrix);
    }
}

DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M) {

    DirectX::XMMATRIX A = M;

    A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    DirectX::XMVECTOR det = XMMatrixDeterminant(A);

    return XMMatrixTranspose(XMMatrixInverse(&det, A));;

}

Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene, const XMMATRIX& transformMatrix)
{
    std::vector<Vertex3D> vertices;
    std::vector<DWORD> indices;

    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex3D vertex;
        vertex.mPos.x = mesh->mVertices[i].x;
        vertex.mPos.y = mesh->mVertices[i].y;
        vertex.mPos.z = mesh->mVertices[i].z;

        if (mesh->mNormals != nullptr)
        {
            vertex.mNormal.x = mesh->mNormals[i].x;
            vertex.mNormal.y = mesh->mNormals[i].y;
            vertex.mNormal.z = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0])
        {
            vertex.mTexCoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.mTexCoord.y = (float)mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    //get indices
    for (UINT i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (UINT j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    std::vector<Texture> textures;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
    textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

    return Mesh(this->mDevice, this->mDeviceContext, vertices, indices, textures, transformMatrix);
}

TextureStorageType Model::DetermineTextureStorageType(const aiScene * pScene, aiMaterial * pMat, unsigned int index, aiTextureType textureType)
{
    if (pMat->GetTextureCount(textureType) == 0)
    {
        return TextureStorageType::None;
    }

    aiString path;
    pMat->GetTexture(textureType, index, &path);
    std::string texturePath = path.C_Str();

    if (texturePath[0] == '*')
    {
        if (pScene->mTextures[0]->mHeight == 0)
        {
            return TextureStorageType::EmbeddedIndexCompressed;
        }
        else
        {
            assert("Support does not exist yet for indexed non compressed textures" && 0);
            return TextureStorageType::EmbeddedIndexNonCompressed;
        }
    }

    if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str()))
    {
        if (pTex->mHeight == 0)
        {
            return TextureStorageType::EmbeddedCompressed;
        }
        else
        {
            assert("Support does not exist yet for embadded non compressed textures" && 0);
            return TextureStorageType::EmbeddedNonCompressed;
        }
    }

    if (texturePath.find('.') != std::string::npos)
    {
        return TextureStorageType::Disk;
    }

    return TextureStorageType::None;
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial * pMaterial, aiTextureType textureType, const aiScene * pScene)
{
    std::vector<Texture> materialTextures;
    TextureStorageType storageType = TextureStorageType::Invalid;
    unsigned int textureCount = pMaterial->GetTextureCount(textureType);

    if (textureCount == 0)
    {
        storageType = TextureStorageType::None;
        aiColor3D aiColor(0.0f, 0.0f, 0.0f);

        switch (textureType)
        {
        case aiTextureType_DIFFUSE:
        {
            pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);

            if (aiColor.IsBlack())
            {
                materialTextures.push_back(Texture(this->mDevice, Colours::UnloadedTextureColor, textureType));
                return materialTextures;
            }

            materialTextures.push_back(Texture(this->mDevice, Color(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType));
            return materialTextures;
        }
        }
    }
    else
    {
        for (UINT i = 0; i < textureCount; i++)
        {
            aiString path;
            pMaterial->GetTexture(textureType, i, &path);
            TextureStorageType storeType = DetermineTextureStorageType(pScene, pMaterial, i, textureType);

            switch (storeType)
            {
            case TextureStorageType::EmbeddedIndexCompressed:
            {
                int index = GetTextureIndex(&path);
                Texture embaddedIndexTexture(this->mDevice,
                                                reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData),
                                                pScene->mTextures[index]->mWidth,
                                                textureType);
                materialTextures.push_back(embaddedIndexTexture);
                break;
            }
            case TextureStorageType::EmbeddedCompressed:
            {
                const aiTexture* pTexture = pScene->GetEmbeddedTexture(path.C_Str());
                Texture embaddedTexture(this->mDevice,
                                        reinterpret_cast<uint8_t*>(pTexture->pcData),
                                        pTexture->mWidth,
                                        textureType);
                materialTextures.push_back(embaddedTexture);
                break;
            }
            case TextureStorageType::Disk:
            {
                std::string fileName = this->mDirectory + '\\' + path.C_Str();
                Texture diskTexture(this->mDevice, fileName, textureType);
                materialTextures.push_back(diskTexture);
                break;
            }
            }
        }
    }

    if (materialTextures.size() == 0)
    {
        materialTextures.push_back(Texture(this->mDevice, Colours::UnhandledTextureColor, aiTextureType_DIFFUSE));
    }

    return materialTextures;

}

int Model::GetTextureIndex(aiString * pStr)
{
    assert(pStr->length >= 2);
    return atoi(&pStr->C_Str()[1]);
}
