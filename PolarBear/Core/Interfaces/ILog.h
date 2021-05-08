#pragma once

#include <cstdint>
#include "../NonCopyable.h"

namespace pb {

    enum class LogLevel : uint32_t {
        None    = 0,
        Raw     = 1 << 0,
        Debug   = 1 << 1,
        Info    = 1 << 2,
        Warning = 1 << 3,
        Error   = 1 << 4,

        All     = 0xffffffff,
    };


    class ILog : NonCopyable<ILog> {
    public:
        ILog() = delete;
        ~ILog() = delete;

        static void Log(LogLevel level, const char* condition, const char* fileName, int lineNumber);
        static void Log(LogLevel level, const char* condition, const char* fileName, int lineNumber, const char* message, ...);
    };
}