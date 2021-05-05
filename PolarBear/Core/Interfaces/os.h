#pragma once

namespace pb
{

    struct Point
    {
        int32_t x;
        int32_t y;
    };
    struct RectDesc
    {
        int32_t left, top, right, bottom;
    };

    inline int GetRectWidth(const RectDesc &rect) { return rect.right - rect.left; }
    inline int GetRectHeight(const RectDesc &rect) { return rect.bottom - rect.top; }

    using WindowHandle = void *;
    constexpr WindowHandle INVALID_WINDOW = nullptr;

    struct WindowDesc
    {
        WindowHandle handle;
        RectDesc windowedRect;
        RectDesc fullScreenRect;
        RectDesc clientRect;
        uint32_t windowsFlags;
        bool fullScreen;
        bool cursorTracked;
        bool iconified;
        bool maximized;
        bool minimized;
        bool hide;
        bool noresizeFrame;
        bool borderlessWindow;
        bool overrideDefaultPosition;
        bool centerd;
        bool forceLowDPI;
    };

    struct Resolution
    {
        uint32_t width;
        uint32_t height;
    };

    struct Event
    {
        enum class Type
        {
            QUIT,
            KEY,
            CHAR,
            MOUSE_BUTTON,
            MOUSE_MOVE,
            MOUSE_WHEEL,
            WINDOW_CLOSE,
            WINDOW_SIZE,
            WINDOW_MOVE,
            DROP_FILE,
            FOCUS
        };

        Type type;
        WindowHandle window;
        union
        {
            //struct { u32 utf8; } text_input;
            struct
            {
                int w, h;
            } win_size;
            struct
            {
                int x, y;
            } win_move;
            //struct { bool down; MouseButton button; } mouse_button;
            struct
            {
                int xrel, yrel;
            } mouse_move;
            //struct { bool down; Keycode keycode; } key;
            struct
            {
                void *handle;
            } file_drop;
            struct
            {
                float amount;
            } mouse_wheel;
            struct
            {
                bool gained;
            } focus;
        };
    };

    using WindowHandle = void *;
    struct InitWindowDesc
    {
        WindowHandle parent;
    };


    // API function
    void RequestShutdown();

    // Window handling
    void OpenWindow(const char *appName, WindowDesc *winDesc);
    void CloseWindow(WindowDesc *winDesc);
    void SetWindowRect(WindowDesc *winDesc, const RectDesc &rect);
    void SetWindowSize(WindowDesc *winDesc, uint32_t width, uint32_t height);
    void ToggleBorderless(WindowDesc *winDesc, uint32_t width, uint32_t height);
    void ToggleFullScreen(WindowDesc *winDesc);
    void ShowWindow(WindowDesc *winDesc);
    void HideWindow(WindowDesc *winDesc);
    void MaximizeWindow(WindowDesc *winDesc);
    void MinimizeWindow(WindowDesc *winDesc);
    void CenterWindow(WindowDesc *winDesc);

    // Mouse and cursor handling
    void* CreateCursor(const char *path);
    void SetCursor(void *cursor);
    void ShowCursor();
    void HideCursor();
    bool IsCursorInsideTrackingArea();
    void SetMousePositionRelative(const WindowDesc *winDesc, int32_t x, int32_t y);
    void SetMousePositionAbsolute(int32_t x, int32_t y);

    void GetRecommendedResolution(RectDesc* rect);

}