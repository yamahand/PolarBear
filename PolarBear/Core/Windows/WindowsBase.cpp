#include <Precompile.h>
#include "../Interfaces/IApp.hpp"

#include <sys/stat.h>
#include <stdlib.h>
#include <algorithm>
#include <chrono>

#include <strconv.h>

#include "../Interfaces/ITime.h"
#include "../Interfaces/IAssert.h"

namespace pb {
    namespace {
        IApp* gApp = nullptr;
        WindowDesc* gWindow = nullptr;
        constexpr auto WINDOW_CLASS_NAME = L"PB";
        bool gCursorVisible = true;
        bool gCursorInsideRectangle = false;
    }

    void AdjustWindow(WindowDesc* winDesc);
    void OnResize(WindowDesc* winDesc, int32_t newSizeX, int32_t newSizeY);

    void GetRecommendedResolution(RectDesc* rect)     {
        *rect = { 0, 0, std::min(1920, (int)(GetSystemMetrics(SM_CXSCREEN) * 0.75f)), std::min(1080,(int)(GetSystemMetrics(SM_CXSCREEN) * 0.75f)) };
    }

    void UpdateWIndowDescfullScreenRect(WindowDesc* winDesc) {
        HMONITOR currentMonitor = MonitorFromWindow((HWND)gApp->pWindow->handle, MONITOR_DEFAULTTONEAREST);
        MONITORINFOEX info = {};
        info.cbSize = sizeof(MONITORINFOEX);
        GetMonitorInfo(currentMonitor, &info);

        winDesc->fullScreenRect.left = info.rcMonitor.left;
        winDesc->fullScreenRect.top = info.rcMonitor.top;
        winDesc->fullScreenRect.right = info.rcMonitor.right;
        winDesc->fullScreenRect.bottom = info.rcMonitor.bottom;
    }

    void UpdateWIndowDescWindowedRect(WindowDesc* winDesc) {
        RECT windowedRect = {};
        HWND hwnd = (HWND)winDesc->handle;
        GetWindowRect(hwnd, &windowedRect);

        winDesc->fullScreenRect.left = windowedRect.left;
        winDesc->fullScreenRect.top = windowedRect.top;
        winDesc->fullScreenRect.right = windowedRect.right;
        winDesc->fullScreenRect.bottom = windowedRect.bottom;
    }

    void UpdateWindowDescClientRect(WindowDesc* winDesc) {
        RECT clientRect = {};
        HWND hwnd = (HWND)winDesc->handle;
        GetClientRect(hwnd, &clientRect);

        winDesc->fullScreenRect.left = clientRect.left;
        winDesc->fullScreenRect.top = clientRect.top;
        winDesc->fullScreenRect.right = clientRect.right;
        winDesc->fullScreenRect.bottom = clientRect.bottom;
    }

    DWORD PrepareStyleMask(WindowDesc* winDesc) {
        DWORD windowStyle = WS_OVERLAPPEDWINDOW;
        if (winDesc->borderlessWindow) {
            windowStyle = WS_POPUP | WS_THICKFRAME;
        }

        if (winDesc->noresizeFrame) {
            windowStyle ^= WS_THICKFRAME | WS_MAXIMIZEBOX;
        }

        if (!winDesc->hide) {
            windowStyle |= WS_VISIBLE;
        }

        return windowStyle;
    }

    void OffsetRectRoDisplay(WindowDesc* winDesc, LPRECT rect) {
        int32_t displayOffsetX = winDesc->fullScreenRect.left;
        int32_t displayOffsetY = winDesc->fullScreenRect.top;

        rect->left += (LONG)displayOffsetX;
        rect->top += (LONG)displayOffsetY;
        rect->right += (LONG)displayOffsetX;
        rect->bottom += (LONG)displayOffsetY;
    }

    void AdjustWindow(WindowDesc* winDesc) {
        HWND hwnd = (HWND)winDesc->handle;

        if (winDesc->fullScreen) {
            RECT windowedRect = {};
            GetWindowRect(hwnd, &windowedRect);
            winDesc->windowedRect = { (int)windowedRect.left, (int)windowedRect.top, (int)windowedRect.right, (int)windowedRect.bottom };

            SetWindowLong(hwnd, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);

            HMONITOR currentMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);
            MONITORINFOEX info = {};
            info.cbSize = sizeof(MONITORINFOEX);
            bool infoRead = GetMonitorInfo(currentMonitor, &info);

            gApp->mSettings.windowX = info.rcMonitor.left;
            gApp->mSettings.windowY = info.rcMonitor.top;

            SetWindowPos(
                hwnd, HWND_NOTOPMOST, info.rcMonitor.left, info.rcMonitor.top, info.rcMonitor.right - info.rcMonitor.left,
                info.rcMonitor.bottom - info.rcMonitor.top, SWP_FRAMECHANGED | SWP_NOACTIVATE
            );

            ShowWindow(hwnd, SW_MAXIMIZE);

            OnResize(winDesc,
                     info.rcMonitor.right - info.rcMonitor.left,
                     info.rcMonitor.bottom - info.rcMonitor.top);
        }
        else {
            auto windowStyle = PrepareStyleMask(winDesc);
            SetWindowLong(hwnd, GWL_STYLE, windowStyle);

            SetWindowPos(
                hwnd,
                HWND_NOTOPMOST,
                winDesc->windowedRect.left,
                winDesc->windowedRect.top,
                GetRectWidth(winDesc->windowedRect),
                GetRectHeight(winDesc->windowedRect),
                SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOOWNERZORDER);

            if (winDesc->maximized) {
                ShowWindow(hwnd, SW_MAXIMIZE);
            }
            else {
                ShowWindow(hwnd, SW_NORMAL);
            }
        }
    }



    void ShowWindow(WindowDesc* winDesc) {
        winDesc->hide = false;
        ShowWindow((HWND)winDesc->handle, SW_SHOW);
    }

    void HideWindow(WindowDesc* winDesc) {
        winDesc->hide = true;
        ShowWindow((HWND)winDesc->handle, SW_HIDE);
    }

    void MaximizeWindow(WindowDesc* winDesc) {
        winDesc->maximized = true;
        ShowWindow((HWND)winDesc->handle, SW_MAXIMIZE);
    }

    void MinimizeWindow(WindowDesc* winDesc) {
        winDesc->minimized = true;
        ShowWindow((HWND)winDesc->handle, SW_MINIMIZE);
    }

    void CenterWindow(WindowDesc* winDesc) {
        UpdateWIndowDescfullScreenRect(winDesc);

        uint32_t fsHalfWidth = GetRectWidth(winDesc->fullScreenRect) >> 1;
        uint32_t fsHalfHeight = GetRectHeight(winDesc->fullScreenRect) >> 1;
        uint32_t windowWidth = GetRectWidth(winDesc->windowedRect);
        uint32_t windowHeight = GetRectHeight(winDesc->windowedRect);
        uint32_t windowHalfWidth = windowWidth >> 1;
        uint32_t windowHalfHeight = windowHeight >> 1;

        uint32_t x = fsHalfWidth - windowHalfWidth;
        uint32_t y = fsHalfHeight - windowHalfHeight;

        RECT rect = {
            (LONG)x,
            (LONG)y,
            (LONG)(x + windowWidth),
            (LONG)(y + windowHeight),
        };

        auto windowStyle = PrepareStyleMask(winDesc);

        AdjustWindowRect(&rect, windowStyle, FALSE);

        OffsetRectRoDisplay(winDesc, &rect);

        SetWindowPos(
            (HWND)winDesc->handle,
            HWND_NOTOPMOST,
            rect.left,
            rect.top,
            rect.right - rect.left,
            rect.bottom - rect.top,
            SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOOWNERZORDER);

        winDesc->windowedRect = {
            (int32_t)rect.left,
            (int32_t)rect.top,
            (int32_t)rect.right,
            (int32_t)rect.bottom,
        };
    }

    void* CreateCursor(const char* path) {
        return LoadCursorFromFileA(path);
    }
    void* CreateCursor(const wchar_t* path) {
        return LoadCursorFromFileW(path);
    }

    void SetCursor(void* cursor) {
        HCURSOR windowCursor = (HCURSOR)cursor;
        SetCursor(windowCursor);
    }

    void ShowCursor() {
        if (!gCursorVisible) {
            gCursorVisible = true;
            ::ShowCursor(TRUE);
        }
    }

    void HideCursor() {
        if (gCursorVisible) {
            gCursorVisible = false;
            ::ShowCursor(FALSE);
        }
    }

    bool IsCursorInsideTrackingArea() {
        return gCursorInsideRectangle;
    }

    void SetMousePositionRelative(const WindowDesc* winDesc, int32_t x, int32_t y) {
        POINT point = { (LONG)x, (LONG)y };
        ClientToScreen((HWND)winDesc->handle, &point);

        ::SetCursorPos(point.x, point.y);
    }

    void SetMousePositionAbsolute(int32_t x, int32_t y) {
        ::SetCursorPos(x, y);
    }

    void SetWindowRect(WindowDesc* winDesc, const RectDesc& rect) {
        RectDesc& currentRect = winDesc->fullScreen ? winDesc->fullScreenRect : winDesc->windowedRect;

        HWND hwnd = static_cast<HWND>(winDesc->handle);
        int clientWidthStart = (GetRectWidth(winDesc->windowedRect) - GetRectWidth(winDesc->clientRect)) >> 1;
        int clientHeightStart = GetRectHeight(winDesc->windowedRect) - GetRectHeight(winDesc->clientRect) - clientWidthStart;

        winDesc->clientRect = rect;

        auto windowStyle = PrepareStyleMask(winDesc);
        SetWindowLong(hwnd, GWL_STYLE, windowStyle);

        if (winDesc->centerd) {
            CenterWindow(winDesc);
        }
        else {
            RECT clientRectSyleAdjusted = {
                (LONG)(rect.left + clientWidthStart),
                (LONG)(rect.top + clientHeightStart),
                (LONG)(clientRectSyleAdjusted.left + GetRectWidth(rect)),
                (LONG)(clientRectSyleAdjusted.top + GetRectHeight(rect))
            };

            AdjustWindowRect(&clientRectSyleAdjusted, windowStyle, FALSE);

            winDesc->windowedRect = {
                (int32_t)clientRectSyleAdjusted.left,
                (int32_t)clientRectSyleAdjusted.top,
                (int32_t)clientRectSyleAdjusted.right,
                (int32_t)clientRectSyleAdjusted.bottom,
            };

            SetWindowPos(
                hwnd,
                HWND_NOTOPMOST,
                clientRectSyleAdjusted.left,
                clientRectSyleAdjusted.top,
                clientRectSyleAdjusted.right - clientRectSyleAdjusted.left,
                clientRectSyleAdjusted.bottom - clientRectSyleAdjusted.top,
                SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOOWNERZORDER
            );
        }
    }

    void SetWindowSize(WindowDesc* winDesc, uint32_t width, uint32_t height) {
        RectDesc newClientRect = {
            newClientRect.left = winDesc->windowedRect.left,
            newClientRect.top = winDesc->windowedRect.top,
            newClientRect.right = newClientRect.left + width,
            newClientRect.bottom = newClientRect.top + height,
        };

        SetWindowRect(winDesc, newClientRect);
    }

    void ToggleFullScreen(WindowDesc* winDesc) {
        winDesc->fullScreen = !winDesc->fullScreen;
        AdjustWindow(winDesc);
    }

    void ToggleBorderless(WindowDesc* winDesc, uint32_t width, uint32_t height) {
        if (!winDesc->fullScreen) {
            winDesc->borderlessWindow = !winDesc->borderlessWindow;

            bool centered = winDesc->centerd;
            winDesc->centerd = false;
            SetWindowSize(winDesc, width, height);
            winDesc->centerd = centered;
        }
    }

    void RequestShutdown() {
        PostQuitMessage(0);
    }

    void OpenWindow(const char* appName, WindowDesc* winDesc) {
        UpdateWIndowDescfullScreenRect(winDesc);

        bool borderless = winDesc->borderlessWindow;
        winDesc->borderlessWindow = false;

        RECT rect = {
            (LONG)winDesc->clientRect.left,
            (LONG)winDesc->clientRect.top,
            (LONG)(winDesc->clientRect.left + winDesc->clientRect.right),
            (LONG)(winDesc->clientRect.top + winDesc->clientRect.bottom),
        };

        auto windowStyle = PrepareStyleMask(winDesc);

        AdjustWindowRect(&rect, windowStyle, false);

        int windowX = rect.left;
        int windowY = rect.top;

        if (!winDesc->overrideDefaultPosition) {
            windowX = CW_USEDEFAULT;
            windowY = CW_USEDEFAULT;
        }

        bool fullScreen = winDesc->fullScreen;
        winDesc->fullScreen = false;

        HWND hwnd = CreateWindowW(
            WINDOW_CLASS_NAME,
            WINDOW_CLASS_NAME,
            windowStyle,
            windowX,
            windowY,
            rect.right - windowX,
            rect.bottom - windowY,
            nullptr,
            nullptr,
            (HINSTANCE)GetModuleHandle(NULL),
            0
        );

        if (hwnd != nullptr) {
            winDesc->handle = hwnd;

            GetClientRect(hwnd, &rect);
            winDesc->clientRect = { (int)rect.left, (int)rect.top, (int)rect.right, (int)rect.bottom };

            GetWindowRect(hwnd, &rect);
            winDesc->windowedRect = { (int)rect.left, (int)rect.top, (int)rect.right, (int)rect.bottom };

            if (!winDesc->hide) {
                if (winDesc->maximized) {
                    ShowWindow(hwnd, SW_MAXIMIZE);
                }
                else if (winDesc->minimized) {
                    ShowWindow(hwnd, SW_MINIMIZE);
                }

                if (borderless) {
                    ToggleBorderless(winDesc, GetRectWidth(winDesc->clientRect), GetRectHeight(winDesc->clientRect));
                }

                if (winDesc->centerd) {
                    CenterWindow(winDesc);
                }

                if (fullScreen) {
                    ToggleFullScreen(winDesc);
                }
            }
        }
        else {

        }

        SetMousePositionRelative(winDesc, GetRectWidth(winDesc->windowedRect) >> 1, GetRectHeight(winDesc->windowedRect) >> 1);
    }

    bool HandleMessage() {
        MSG msg = {};
        bool quit = false;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_CLOSE || msg.message == WM_QUIT) {
                quit = true;
            }
        }

        return quit;
    }

    void CloseWindow(const WindowDesc* winDesc) {
        DestroyWindow((HWND)winDesc->handle);
        HandleMessage();
    }

    void OnResize(WindowDesc* winDesc, int32_t newSizeX, int32_t newSizeY) {
        if (!gApp || !gApp->mSettings.initialized)
            return;

        gApp->mSettings.fullScreen = winDesc->fullScreen;
        if (gApp->mSettings.width == newSizeX && gApp->mSettings.height == newSizeY) {
            return;
        }

        gApp->mSettings.width = newSizeX;
        gApp->mSettings.height = newSizeY;

    }

    inline float CounterToSecondsElapsed(int64_t start, int64_t end) {
        return (float)(end - start) / (float)1e6;
    }


    LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if (!gWindow) {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }

        switch (message) {
        case WM_DESTROY:
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        return 0;
    }

    int WindowsMain(int argc, const char** argv, IApp* app) {

        //PB_ASSERT(false);
        PB_ASSERT_MSG(false, "%d, {0}", argc, argc);
        WindowDesc window = {};

        static WNDCLASS wc = [&]() -> WNDCLASS {
            WNDCLASS wc = {};

            wc.style = 0;
            wc.lpfnWndProc = WndProc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hInstance = GetModuleHandle(NULL);
            wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.hbrBackground = NULL;
            wc.lpszClassName = WINDOW_CLASS_NAME;

            if (!RegisterClass(&wc)) {
                //ASSERT(false);
                return {};
            }
            return wc;
        }();

        gApp = app;

        IApp::Settings* pSettings = &gApp->mSettings;
        gWindow = &window;
        gApp->pWindow = &window;

        if (pSettings->width <= 0 || pSettings->height <= 0) {
            RectDesc rect = {};

            GetRecommendedResolution(&rect);
            pSettings->width = GetRectWidth(rect);
            pSettings->height = GetRectHeight(rect);
        }

        gWindow->clientRect = {
            pSettings->windowX,
            pSettings->windowY,
            pSettings->width,
            pSettings->height,
        };

        gWindow->windowedRect = gWindow->clientRect;
        gWindow->fullScreen = pSettings->fullScreen;
        gWindow->maximized = false;
        gWindow->noresizeFrame = !pSettings->dragToResize;
        gWindow->borderlessWindow = pSettings->borderlessWindow;
        gWindow->centerd = pSettings->centerd;
        gWindow->forceLowDPI = pSettings->forceLowDPI;
        gWindow->overrideDefaultPosition = true;

        OpenWindow(gApp->GetName(), gApp->pWindow);

        gWindow->handle = gApp->pWindow->handle;

        pSettings->width = gApp->pWindow->fullScreen ? GetRectWidth(gApp->pWindow->fullScreenRect) : GetRectWidth(gApp->pWindow->clientRect);
        pSettings->height = gApp->pWindow->fullScreen ? GetRectHeight(gApp->pWindow->fullScreenRect) : GetRectHeight(gApp->pWindow->clientRect);

        gApp->pCommandLine = GetCommandLineA();
        {
            if (!gApp->Init()) {
                return EXIT_FAILURE;
            }

            pSettings->initialized = true;
        }

        auto lastCounter = GetUSec();
        bool quit = false;
        while (!quit) {
            auto counter = GetUSec();
            float deltaTime = CounterToSecondsElapsed(lastCounter, counter);
            lastCounter = counter;

            quit = HandleMessage() || pSettings->quit;

            if (gWindow->minimized) {
                Sleep(1);
                continue;
            }

            gApp->Update(deltaTime);
            gApp->Draw();
        }

        gApp->mSettings.quit = true;
        gApp->Exit();

        return 0;
    }
}