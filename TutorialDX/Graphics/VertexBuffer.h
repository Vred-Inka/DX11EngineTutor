#ifndef VertexBuffer_h_
#define VertexBuffer_h_
#include <d3d11.h>
#include <wrl/client.h>

template<class T>
class VertexBuffer
{
private:
    VertexBuffer(const VertexBuffer<T>& rhs);

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
    std::unique_ptr<UINT> mStride;
    UINT mBufferSize = 0;

public:
    VertexBuffer() {};

    ID3D11Buffer* Get() const
    {
        return mBuffer.Get();
    }

    ID3D11Buffer* const* GetAddressOf() const
    {
        return mBuffer.GetAddressOf();
    }

    UINT BufferSize() const
    {
        return this->mBufferSize;
    }

    const UINT Stride() const
    {
        return *this->mStride.get();
    }

    const UINT* StridePtr() const
    {
        return this->mStride.get();
    }

    HRESULT Initialize(ID3D11Device* device, T* data, UINT numVertices)
    {
        if (this->mBuffer.Get() != nullptr)
        {
            this->mBuffer.Reset();
        }

        this->mBufferSize = numVertices;

        if (this->mStride.get() == nullptr)
        {
            this->mStride = std::make_unique<UINT>(sizeof(T));
        }

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(T) * numVertices;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexBufferData;
        ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
        vertexBufferData.pSysMem = data;

        HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->mBuffer.GetAddressOf());
        return hr;        
    }
};
#endif