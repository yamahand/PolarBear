#include "Precompile.h"
#include "Singleton.h"

#include "Interfaces/IAssert.h"

namespace pb {

    namespace {
        constexpr size_t kMaxFinalizersSize = 256;
        std::mutex gMutex;
        size_t gNumFinalizersSize = 0;
        SingletonFinalizer::FinalizeFunc gFinalizers[kMaxFinalizersSize] = {};
    }


    void SingletonFinalizer::AddFinalizer(FinalizeFunc func) {
        std::lock_guard<std::mutex> lock(gMutex);
        PB_ASSERT(gNumFinalizersSize < kMaxFinalizersSize);
        gFinalizers[gNumFinalizersSize++] = func;
    }

    void SingletonFinalizer::Finalize() {
        std::lock_guard<std::mutex> lock(gMutex);
        for (size_t i = gNumFinalizersSize - 1; i >= 0; --i) {
            (*gFinalizers[i])();
        }
        gNumFinalizersSize = 0;
    }
}