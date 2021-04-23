#ifndef VertexBuffer_h_
#define VertexBuffer_h_
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

template<class T>
class VertexBuffer
{
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
    UINT mStride = sizeof(T); 
    UINT mVertexCount = 0;

public:
    VertexBuffer() {};

    VertexBuffer(const VertexBuffer<T>& rhs)
    {
        mBuffer = rhs.mBuffer;
        mVertexCount = rhs.mVertexCount;
        mStride = rhs.mStride;
    }

    VertexBuffer<T> & operator= (const VertexBuffer<T>& rhs)
    {
        mBuffer = rhs.mBuffer;
        mVertexCount = rhs.mVertexCount;
        mStride = rhs.mStride;
        return *this;
    }

    ID3D11Buffer* Get() const
    {
        return mBuffer.Get();
    }

    ID3D11Buffer* const* GetAddressOf() const
    {
        return mBuffer.GetAddressOf();
    }

    UINT VertexCount() const
    {
        return this->mVertexCount;
    }

    const UINT Stride() const
    {
        return this->mStride;
    }

    const UINT* StridePtr() const
    {
        return &this->mStride;
    }

    HRESULT Initialize(ID3D11Device* device, T* data, UINT vertexCount)
    {
        if (this->mBuffer.Get() != nullptr)
        {
            this->mBuffer.Reset();
        }

        this->mVertexCount = vertexCount;

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = mStride * vertexCount;
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