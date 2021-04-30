#include <Windows.h>
#include <tchar.h>
#include <PolarBear.hpp>
#include <Core/Engine.hpp>


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    pb::output("Hello world.\n");
    std::wstring wstr = L"Helo world. wstr";
    pb::output(wstr);

    pb::Engine engine;
    engine.Init();

    while (1) {
        Sleep(100);
    }

    return 0;
}
