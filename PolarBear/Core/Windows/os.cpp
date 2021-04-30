#include "Precompile.h"
#include "../os.h"

#include <Windows.h>
#include <string>

namespace pb {

    WindowHandle InitWindow(const InitWindowDesc& desc) {
        std::wstring cls_name(L"PolarBear");
        static WNDCLASS wc = [&]() -> WNDCLASS {
            WNDCLASS wc = {};
            auto WndProc = [](HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) -> LRESULT {
                Event e;
                e.window = hWnd;
                switch (Msg) {
                case WM_MOVE:
                    e.type = Event::Type::WINDOW_MOVE;
                    e.win_move.x = (int)LOWORD(lParam);
                    e.win_move.y = (int)HIWORD(lParam);
                    //G.event_queue.pushBack(e);
                    //updateGrabbedMouse();
                    return 0;
                case WM_SIZE:
                    e.type = Event::Type::WINDOW_SIZE;
                    e.win_size.w = LOWORD(lParam);
                    e.win_size.h = HIWORD(lParam);
                    //G.event_queue.pushBack(e);
                    //updateGrabbedMouse();
                    return 0;
                case WM_CLOSE:
                    e.type = Event::Type::WINDOW_CLOSE;
                    //G.event_queue.pushBack(e);
                    //if (hWnd == G.grabbed_window) G.grabbed_window = INVALID_WINDOW;
                    //updateGrabbedMouse();
                    return 0;
                case WM_ACTIVATE:
                    if (wParam == WA_INACTIVE) {
                        //showCursor(true);
                        //grabMouse(INVALID_WINDOW);
                    }

                    e.type = Event::Type::FOCUS;
                    e.focus.gained = wParam != WA_INACTIVE;
                    //G.event_queue.pushBack(e);
                    //updateGrabbedMouse();
                    break;
                }
                return DefWindowProc(hWnd, Msg, wParam, lParam);
            };

            wc.style = 0;
            wc.lpfnWndProc = WndProc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hInstance = GetModuleHandle(NULL);
            wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wc.hCursor = NULL;
            wc.hbrBackground = NULL;
            wc.lpszClassName = cls_name.data();

            if (!RegisterClass(&wc)) {
                //ASSERT(false);
                return {};
            }
            return wc;
        }();

        HWND parent_window = (HWND)desc.parent;

        //WCharStr<LUMIX_MAX_PATH> wname(args.name);
        //DWORD style =  args.flags & InitWindowArgs::NO_DECORATION ? WS_POPUP : WS_OVERLAPPEDWINDOW ;
        //DWORD ext_style = args.flags & InitWindowArgs::NO_TASKBAR_ICON ? WS_EX_TOOLWINDOW : WS_EX_APPWINDOW;
        const HWND hwnd = CreateWindowEx(
            WS_EX_APPWINDOW,//ext_style,
            cls_name.data(),
            L"PB",
            WS_OVERLAPPEDWINDOW,//style,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            parent_window,
            NULL,
            wc.hInstance,
            NULL);
        //ASSERT(hwnd);

        //if (args.handle_file_drops)
        //{
        //	DragAcceptFiles(hwnd, TRUE);
        //}

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        //if (!G.raw_input_registered) {
        //	RAWINPUTDEVICE device;
        //	device.usUsagePage = 0x01;
        //	device.usUsage = 0x02;
        //	device.dwFlags = RIDEV_INPUTSINK;
        //	device.hwndTarget = hwnd;
        //	BOOL status = RegisterRawInputDevices(&device, 1, sizeof(device));
        //	//ASSERT(status);
        //	G.raw_input_registered = true;
        //}

        return hwnd;
    }

}