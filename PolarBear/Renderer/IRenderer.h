#pragma once

#include "../Core/Interfaces/os.h"

#if defined (PB_DX12)
#include <d3d12.h>
#include <dxgi1_6.h>
#endif

namespace D3D12MA {
    class Allocator;
    class Allocation;
}

namespace pb{

    enum class RendererApi {
        D3D12,
        D3D11,
        VULKAN,
    };


    struct RendererDesc {

    };

    struct SwapChainDesc {
        WindowHandle    windowHandle;

    };

    enum class FenceStatus {
        Complete,
        Incomplete,
        Notsubmitted,
    };

    struct Fence {
#if defined(PB_DX12)
        ID3D12Fence*    pFence;
        HANDLE          pDxWaitIdleFenceEvent;
        uint64_t        fenceValue;
#endif
    };

    struct alignas(64) Renderer{
#if defined(PB_DX12)
        struct DescriptorHeap**     pCPUDescriptorHeaps;
        struct DescriptorHeap**     pCbvSrvUavHeaps;
        struct DescriptorHeap**     pSamplerHeaps;
        class  D3D12MA::Allocator*  pResourceAllocator;
        IDXGIFactory6*              pDxgiFactory;
        IDXGIAdapter4*              pDxAdapter;
        ID3D12Device*               pDevice;
        ID3D12Debug*                pDxDebug;
        ID3D12InfoQueue*            pDxDebugCalidation;
#endif

        struct NullDescriptors*     pNullDescriptors;
        char*                       pName;
    };

    static_assert(sizeof(Renderer) <= 24 * sizeof(uint64_t), "sizeof(Renderer) <= 24 * sizeof(uint64_t)");

    void InitRenderer(const char* appName, const RendererDesc& rendererDesc, Renderer** pRenderer);
    void RemoveRenderer(Renderer* pRenderer);
}