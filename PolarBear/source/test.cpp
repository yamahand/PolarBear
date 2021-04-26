#include <PolarBear.hpp>
#include <iostream>
#include <Windows.h>

#include <strconv.h>
#include <fmt/format.h>

namespace pb {
    void output(const char* str) {
        std::cout << str << std::endl;
        OutputDebugStringA(str);
        std::string s = str;
        auto w = ansi_to_wide(s);
        OutputDebugString(w.c_str());
        auto fm = fmt::format("{}", str);
        OutputDebugStringA(fm.c_str());
    }
}