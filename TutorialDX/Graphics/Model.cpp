#include "Model.h"
bool Model::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
    this->device = device;
    this->deviceContext = deviceContext;
    this->texture = texture;
    this->cb_vs_vertexshader = &cb_vs_vertexshader;

    try
    {
        if (!this->LoadModel(filePath))
        {
            return false;
        }
        /*
        {
            //Textured Square
            Vertex v[] =
            {
                Vertex(-0.5f,  -0.5f, -0.5f, 0.0f, 1.0f), //FRONT Bottom Left   - [0]
                Vertex(-0.5f,   0.5f, -0.5f, 0.0f, 0.0f), //FRONT Top Left      - [1]
                Vertex(0.5f,   0.5f, -0.5f, 1.0f, 0.0f), //FRONT Top Right     - [2]
                Vertex(0.5f,  -0.5f, -0.5f, 1.0f, 1.0f), //FRONT Bottom Right   - [3]
                Vertex(-0.5f,  -0.5f, 0.5f, 0.0f, 1.0f), //BACK Bottom Left   - [4]
                Vertex(-0.5f,   0.5f, 0.5f, 0.0f, 0.0f), //BACK Top Left      - [5]
                Vertex(0.5f,   0.5f, 0.5f, 1.0f, 0.0f), //BACK Top Right     - [6]
                Vertex(0.5f,  -0.5f, 0.5f, 1.0f, 1.0f), //BACK Bottom Right   - [7]
            };

            //Load Vertex Data
            HRESULT hr = this->vertexBuffer.Initialize(this->device, v, ARRAYSIZE(v));
            COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer.");

            DWORD indices[] =
            {
                0, 1, 2, //FRONT
                0, 2, 3, //FRONT
                4, 7, 6, //BACK 
                4, 6, 5, //BACK
                3, 2, 6, //RIGHT SIDE
                3, 6, 7, //RIGHT SIDE
                4, 5, 1, //LEFT SIDE
                4, 1, 0, //LEFT SIDE
                1, 5, 6, //TOP
                1, 6, 2, //TOP
                0, 3, 7, //BOTTOM
                0, 7, 4, //BOTTOM
            };

            //Load Index Data
            hr = this->indexBuffer.Initialize(this->device, indices, ARRAYSIZE(indices));
            COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer.");
        }
        */
    }
    catch (COMException & exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }

    return true;
}

void Model::SetTexture(ID3D11ShaderResourceView * texture)
{
    this->texture = texture;
}

void Model::Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewProjectionMatrix)
{
    //Update Constant buffer with WVP Matrix
    this->cb_vs_vertexshader->data.mat = worldMatrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
    this->cb_vs_vertexshader->data.mat = XMMatrixTranspose(this->cb_vs_vertexshader->data.mat);
    this->cb_vs_vertexshader->ApplyChanges();
    this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());

    this->deviceContext->PSSetShaderResources(0, 1, &this->texture); //Set Texture

    for (int i = 0; i < mMeshes.size(); i++)
    {
        mMeshes[i].Draw();
    }
    /*this->deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
    UINT offset = 0;
    this->deviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
    this->deviceContext->DrawIndexed(this->indexBuffer.BufferSize(), 0, 0); //Draw
    */
}

bool Model::LoadModel(const std::string & filePath)
{
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(filePath,
                                                aiProcess_Triangulate | 
                                               aiProcess_ConvertToLeftHanded);

    if (pScene == nullptr)
    {
        return false;
    }

    this->ProcessNode(pScene->mRootNode, pScene);
    return true;
}

void Model::ProcessNode(aiNode* node, const aiScene * scene)
{
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        mMeshes.push_back(this->ProcessMesh(mesh, scene));
    }

    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        this->ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
    std::vector<Vertex> vertices;
    std::vector<DWORD> indices;

    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.mPos.x = mesh->mVertices[i].x;
        vertex.mPos.y = mesh->mVertices[i].y;
        vertex.mPos.z = mesh->mVertices[i].z;

        if (mesh->mTextureCoords[0])
        {
            vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
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

    return Mesh(this->device, this->deviceContext, vertices, indices);
}