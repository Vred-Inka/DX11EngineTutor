#include "AdaptorReader.h"

std::vector<AdapterData> AdapterReader::mAdapters;

std::vector<AdapterData> AdapterReader::GetAdapters()
{
    if (mAdapters.size() > 0)
    {
        return mAdapters;
    }

    Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create DXGIFactory for enumerating adapters.");
        exit(-1);
    }

    IDXGIAdapter* pAdapter;
    UINT index = 0;
    while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
    {
        mAdapters.push_back(AdapterData(pAdapter));
        index += 1;
    }

    return mAdapters;
}

AdapterData::AdapterData(IDXGIAdapter * pAdapter)
{
    this->pAdapter = pAdapter;
    HRESULT hr = pAdapter->GetDesc(&this->mDescription);
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to Get Description for IDXGIAdapter.");
    }
}
