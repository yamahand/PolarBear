#pragma once

#include <string>
#include <fmt/core.h>
#include <fmt/format.h>

namespace pb {

    void output(std::string_view str);
    void output(std::wstring_view str);

    template <class... Args>
    std::string format(std::string_view fmt, const Args &...args) {
        char buffer[256] = {};
        fmt::vformat_to_n(buffer, 256, fmt::basic_string_view<char>(fmt), fmt::make_format_args(args...));
        return fmt::vformat(fmt, { fmt::make_format_args(args...) });
        //return fmt::format("The answer is {}.", 42);
    }

    template <class... Args>
    std::wstring format(std::wstring_view fmt, const Args &...args) {
        return fmt::vformat(fmt, { fmt::make_format_args(args...) });
    }

    template <class... Args>
    std::string format(const std::locale& loc, std::string_view fmt, const Args &...args) {
        return fmt::vformat(loc, fmt, { fmt::make_format_args(args...) });
    }

    template <class... Args>
    std::wstring format(const std::locale& loc, std::wstring_view fmt, const Args &...args) {
        return fmt::vformat(loc, fmt, { fmt::make_format_args(args...) });
    }

    template <class... Args>
    void outputf(std::string_view fmt, const Args &...args) {
        auto str = format(fmt, args...);
        output(str);
    }
}
