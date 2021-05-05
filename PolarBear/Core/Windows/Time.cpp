#include <Precompile.h>

#include "../Interfaces/ITime.h"

#include <stdint.h>
#include <windows.h>

namespace pb {
    namespace {
        int64_t highResTimerFrequency = 0;
    }

    void InitTime() {
        LARGE_INTEGER frequency;
        if (QueryPerformanceFrequency(&frequency)) {
            highResTimerFrequency = frequency.QuadPart;
        }
        else {
            highResTimerFrequency = 1000LL;
        }
    }

    int64_t GetUSec() {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        return counter.QuadPart * (int64_t)1e6 / GetTimeFrequency();
    }

    int64_t GetTimeFrequency() {
        if (highResTimerFrequency == 0) {
            InitTime();
        }
        return highResTimerFrequency;
    }
}