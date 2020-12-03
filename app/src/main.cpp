#include <Windows.h>
#include <tchar.h>
#include <PolarBear.hpp>


int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {
    pb::output("Hello world.");
    return 0;
}