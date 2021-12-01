#include <Precompile.h>
#include <PolarBear.hpp>
#include <iostream>
#include <format>
#include <Windows.h>

#include <strconv.h>

namespace pb {
    void output(std::string_view str) {
        std::cout << str << std::endl;
        OutputDebugStringA(str.data());
        std::string s = str.data();
        auto w = ansi_to_wide(s);
        OutputDebugString(w.c_str());
        auto fm = std::format("{}", str);
        OutputDebugStringA(fm.c_str());
    }
    void output(std::wstring_view str) {
        OutputDebugString(str.data());
        auto u8str = wide_to_ansi(str.data());
        auto fm = std::format("{}", u8str);
        //OutputDebugStringA(fm.data());
    }
    void output(const char* str) {
        std::cout << str << std::endl;
        OutputDebugStringA(str);
        std::string s = str;
        auto w = ansi_to_wide(s);
        OutputDebugString(w.c_str());
        auto fm = std::format("{}", str);
        OutputDebugStringA(fm.c_str());
    }

}