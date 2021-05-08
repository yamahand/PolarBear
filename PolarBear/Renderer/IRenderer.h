#pragma once

namespace pb{

    struct alignas(64) Renderer{
        struct DescriptorHeap** pCPUDescriptorHeaps;
        struct DescriptorHeap** pCbvSrvUavHeaps;
        struct DescriptorHeap** pSamplerHeaps;
        //class  D3D12MA::Allocator* pResourceAllocator;
    };

    void InitRenderer();
}