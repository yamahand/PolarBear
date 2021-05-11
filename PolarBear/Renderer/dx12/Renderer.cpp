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

#include "strconv.h"
#include "Core/Interfaces/IAssert.h"

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
        PB_ASSERT(SUCCEEDED(result));

        //DirectX12まわり初期化
        //フィーチャレベル列挙
        D3D_FEATURE_LEVEL levels[] = {
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        //Direct3Dデバイスの初期化
        D3D_FEATURE_LEVEL featureLevel;
        for (auto l : levels) {
            if (D3D12CreateDevice(nullptr, l, IID_PPV_ARGS(&device)) == S_OK) {
                featureLevel = l;
                break;
            }
        }

        // アダプター列挙用
        std::vector<IDXGIAdapter*> adapters;

        // ここに特定の名前を持つアダプターオブジェクトが入る
        IDXGIAdapter* tmpAdapter = nullptr;

        for (UINT i = 0; dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)     {
            adapters.push_back(tmpAdapter);
        }

        for (auto& adapter : adapters)     {
            DXGI_ADAPTER_DESC desc = {};
            adapter->GetDesc(&desc);

            std::wstring strDesc = desc.Description;

            auto&& str = wide_to_ansi(strDesc);
            PB_TRACE("%ls\n", str.c_str());
        }

        result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));
        PB_ASSERT(SUCCEEDED(result));

        result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator, nullptr, IID_PPV_ARGS(&cmdList));
        PB_ASSERT(SUCCEEDED(result));

        D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
        cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        cmdQueueDesc.NodeMask = 0;
        cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        result = device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
        assert(cmdQueue);
        assert(SUCCEEDED(result));

        DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
        swapchainDesc.Width = windowWidth;
        swapchainDesc.Height = windowHeight;
        swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapchainDesc.Stereo = false;
        swapchainDesc.SampleDesc.Count = 1;
        swapchainDesc.SampleDesc.Quality = 0;
        swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
        swapchainDesc.BufferCount = 2;
        swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        result = dxgiFactory->CreateSwapChainForHwnd(
            cmdQueue,
            hwnd,
            &swapchainDesc,
            nullptr,
            nullptr,
            (IDXGISwapChain1**)&swapchain
        );
        assert(SUCCEEDED(result));

        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heapDesc.NodeMask = 0;
        heapDesc.NumDescriptors = 2;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        ID3D12DescriptorHeap* rtvHeap = nullptr;
        result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap));
        assert(SUCCEEDED(result));

        DXGI_SWAP_CHAIN_DESC swcDesc = {};
        result = swapchain->GetDesc(&swcDesc);
        assert(SUCCEEDED(result));

        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

        std::vector<ID3D12Resource*> backBuffers(swcDesc.BufferCount);
        D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

        for (UINT idx = 0; idx < swcDesc.BufferCount; idx++) {
            result = swapchain->GetBuffer(idx, IID_PPV_ARGS(&backBuffers[idx]));
            rtvDesc.Format = backBuffers[idx]->GetDesc().Format;
            device->CreateRenderTargetView(backBuffers[idx], &rtvDesc, handle);
            assert(SUCCEEDED(result));

            handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        }

        // 深度バッファの作成
        D3D12_RESOURCE_DESC depthResDesc = {};
        depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        depthResDesc.Width = windowWidth;
        depthResDesc.Height = windowHeight;
        depthResDesc.DepthOrArraySize = 1;
        depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthResDesc.SampleDesc.Count = 1;
        depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        // 深度値用ヒーププロパティ
        D3D12_HEAP_PROPERTIES depthHeapProp = {};
        depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
        depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

        // クリアバリュー
        CD3DX12_CLEAR_VALUE depthClearValue(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

        ID3D12Resource* depthBuffer = nullptr;
        result = device->CreateCommittedResource(
            &depthHeapProp,
            D3D12_HEAP_FLAG_NONE,
            &depthResDesc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &depthClearValue,
            IID_PPV_ARGS(&depthBuffer));
        assert(SUCCEEDED(result));

        // 深度のためのディスクリプタヒープ
        D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
        dsvHeapDesc.NumDescriptors = 1;
        dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        ID3D12DescriptorHeap* dsvHeap = nullptr;
        result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
        assert(SUCCEEDED(result));

        // 深度ビューの作成
        D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
        dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

        device->CreateDepthStencilView(
            depthBuffer,
            &dsvDesc,
            dsvHeap->GetCPUDescriptorHandleForHeapStart()
        );

    }

}