#pragma once

#include "os.h"

namespace pb {
 
    class IApp {
    public:
        virtual bool Init() = 0;
        virtual void Exit() = 0;

        virtual void Update(float deltaTime) = 0;
        virtual void Draw() = 0;

        virtual const char* GetName() = 0;

        struct Settings {
            int32_t width = -1;
            int32_t height = -1;
            int32_t windowX = 0;
            int32_t windowY = 0;
            bool fullScreen = false;
            bool dragToResize = false;
            bool borderlessWindow = false;
            bool centerd = false;
            bool forceLowDPI = false;
            bool initialized = false;
            bool quit = false;
            bool resetGraphics = false;
        } mSettings;

        WindowDesc* pWindow = nullptr;
        const char* pCommandLine = nullptr;

        static int argc;
        static const char** argv;
    };

#if defined (PB_WINDOWS)
    extern int WindowsMain(int argc, const char** argv, IApp* app);
#define DEFINE_APPLiCATION_MAIN(appClass)                   \
    int pb::IApp::argc;                                     \
    const char** pb::IApp::argv;                            \
                                                            \
    int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {  \
        appClass app;                                       \
        return pb::WindowsMain(0, nullptr, &app);               \
    }
#endif
}