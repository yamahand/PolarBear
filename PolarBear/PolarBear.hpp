#pragma once

#include <string>
#include <format>

namespace pb {

    void output(std::string_view str);
    void output(std::wstring_view str);

    template <class... Args>
    std::string format(std::string_view std, const Args &...args) {
        char buffer[256] = {};
        std::format_to_n(buffer, 256, std::basic_string_view<char>(std), std::make_format_args(args...));
        return std::vformat(std, { std::make_format_args(args...) });
        //return std::format("The answer is {}.", 42);
    }

    template <class... Args>
    std::wstring format(std::wstring_view std, const Args &...args) {
        return std::vformat(std, { std::make_format_args(args...) });
    }

    template <class... Args>
    std::string format(const std::locale& loc, std::string_view std, const Args &...args) {
        return std::vformat(loc, std, { std::make_format_args(args...) });
    }

    template <class... Args>
    std::wstring format(const std::locale& loc, std::wstring_view std, const Args &...args) {
        return std::vformat(loc, std, { std::make_format_args(args...) });
    }

    template <class... Args>
    void outputf(std::string_view std, const Args &...args) {
        auto str = format(std, args...);
        output(str);
    }
}
