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
    int IApp::argc;
    const char** IApp::argv;
#endif
}