#include "Precompile.h"
#include "MemoryManager.h"

namespace pb {

    void* MemoryManager::Allocate(size_t size) {
        return malloc(size);
    }

    void MemoryManager::Deallocate(void* ptr) {
        free(ptr);
    }

}