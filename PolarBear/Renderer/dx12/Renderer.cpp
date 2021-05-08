#include <Precompile.h>
#include "../IRenderer.h"

#include <wrl.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>

#define D3D12_GPU_VIRTUAL_ADDRESS_NULL      ((D3D12_GPU_VIRTUAL_ADDRESS)0)
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN   ((D3D12_GPU_VIRTUAL_ADDRESS)-1)

#include "d3dx12.h"

namespace pb {

    using namespace DirectX;
    using namespace Microsoft::WRL;

    ID3D12Device* device = nullptr;
    IDXGIFactory6* dxgiFactory = nullptr;
    IDXGISwapChain4* swapchain = nullptr;
    ID3D12CommandAllocator* cmdAllocator = nullptr;
    ID3D12GraphicsCommandList* cmdList = nullptr;
    ID3D12CommandQueue* cmdQueue = nullptr;

    void EnableDebugLayer() {
        ID3D12Debug* debugLayer = nullptr;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)))) {
            debugLayer->EnableDebugLayer();
            debugLayer->Release();
        }
    }

    void InitRenderer() {

#ifdef PB_DEBUG
        EnableDebugLayer();
        auto result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&dxgiFactory));
#else
        auto result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
#endif
        assert(SUCCEEDED(result));
    }

}