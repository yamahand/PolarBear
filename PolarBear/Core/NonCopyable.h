#pragma once

namespace pb {
    template <class Derrived>
    class NonCopyable {
        NonCopyable() = default;
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable) = delete;
    };
}