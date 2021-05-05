#include <Windows.h>
#include <tchar.h>
#include <PolarBear.hpp>
#include <Core/Engine.hpp>
#include <Core/Interfaces/IApp.hpp>

class App : public pb::IApp {
public:
    App() {
        mSettings.width = 1280;
        mSettings.height = 720;
        mSettings.windowX = 100;
        mSettings.windowY = 100;
        //mSettings.dragToResize = true;
        //mSettings.centerd = true;
    }
    bool Init() override {
        return true;
    }
    void Exit()  override {

    }

    void Update(float deltaTime) override {

    }
    void Draw() override {

    }

    const char* GetName() override {
        return "app";
    }
};


//int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
//    pb::output("Hello world.\n");
//    std::wstring wstr = L"Helo world. wstr";
//    pb::output(wstr);
//
//    pb::Engine engine;
//    engine.Init();
//
//    engine.Run();
//
//    engine.Term();
//    return 0;
//}

DEFINE_APPLiCATION_MAIN(App);