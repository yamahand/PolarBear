#include <Windows.h>
#include <tchar.h>
#include <PolarBear.hpp>


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    pb::output("Hello world.\n");
    std::wstring wstr = L"Helo world. wstr";
    pb::output(wstr);

    return 0;
}
