#pragma once

namespace pb {

    struct Point { int32_t x; int32_t y; };
    struct RectDesc { int32_t left, top, right, bottom; };

    inline int GetRectWidth(const RectDesc& rect) { return rect.right - rect.left; }
    inline int GetRectHeight(const RectDesc& rect) { return rect.bottom - rect.top; }

    using WindowHandle = void*;
    constexpr WindowHandle INVALID_WINDOW = nullptr;

    struct WindowDesc {
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

    struct Event {
        enum class Type {
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
        union {
            //struct { u32 utf8; } text_input;
            struct { int w, h; } win_size;
            struct { int x, y; } win_move;
            //struct { bool down; MouseButton button; } mouse_button;
            struct { int xrel, yrel; } mouse_move;
            //struct { bool down; Keycode keycode; } key;
            struct { void* handle; } file_drop;
            struct { float amount; } mouse_wheel;
            struct { bool gained; } focus;
        };
    };

    using WindowHandle = void*;
    struct InitWindowDesc {
        WindowHandle parent;
    };

    WindowHandle InitWindow(const InitWindowDesc& desc);
    bool GetEvent(Event& e);
}