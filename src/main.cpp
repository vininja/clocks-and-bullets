#undef UNICODE
#define NOMINMAX
#include <windows.h>
#include "application.hpp"

Application::t_pressed_key g_pressed_keys = 0;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        fill_rect(0, 0, sim::screen_w, sim::screen_h, 0);
        break;
    case WM_KEYDOWN:
        switch(wParam)
        {
            case VK_LEFT:  g_pressed_keys |= Application::s_key_left;  break;
            case VK_UP:    g_pressed_keys |= Application::s_key_up;    break;
            case VK_RIGHT: g_pressed_keys |= Application::s_key_right; break;
            case VK_DOWN:  g_pressed_keys |= Application::s_key_down;  break;
            case VK_SPACE: g_pressed_keys |= Application::s_key_space; break;
            case VK_ESCAPE: PostQuitMessage(0); break;
        }
        break;
    case WM_KEYUP:
        switch(wParam)
        {
            case VK_LEFT:  g_pressed_keys &= ~Application::s_key_left;  break;
            case VK_UP:    g_pressed_keys &= ~Application::s_key_up;    break;
            case VK_RIGHT: g_pressed_keys &= ~Application::s_key_right; break;
            case VK_DOWN:  g_pressed_keys &= ~Application::s_key_down;  break;
            case VK_SPACE: g_pressed_keys &= ~Application::s_key_space; break;
        }
        break;
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    s_init	init;
    memset(&init, 0, sizeof(init));

    init.captionText  = "test";
    init.instance     = hInstance;
    init.wndProc      = WndProc;
    init.screen_width  = static_cast<int>(sim::screen_w);
    init.screen_height = static_cast<int>(sim::screen_h);

    HWND hwnd_main = InitializeGraphics( init );
    
    clear_screen();

    {
        Application app(sim::screen_w, sim::screen_h);
        DWORD last_time = timeGetTime();
        
        // Main game loop

        while(true)
        {
            const DWORD k_sleep_time_ms = 16;			
            const DWORD curr_time = timeGetTime();
            const DWORD time_since_last = curr_time - last_time;

            if (time_since_last < k_sleep_time_ms)
            {
                Sleep(k_sleep_time_ms - time_since_last);
            }

            last_time = curr_time;			

            // Windows messaging

            MSG msg;
            if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if(msg.message==WM_QUIT)
                {
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
                
            app.tick(g_pressed_keys);
        }
    }

    UninitializeGraphics();

    return 0;
}
