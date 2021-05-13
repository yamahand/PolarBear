#pragma once

namespace pb {
    class IAllocator {
    public:
        virtual void* Allocate(size_t size) = 0;
        virtual void Deallocate(void* ptr) = 0;
    };
}