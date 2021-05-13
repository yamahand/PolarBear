#pragma once

#include "../Interfaces/IAllocator.h"

namespace pb {
    class MemoryManager : public IAllocator {
    public:
        virtual void* Allocate(size_t size);
        virtual void Deallocate(void* ptr);

    };
}