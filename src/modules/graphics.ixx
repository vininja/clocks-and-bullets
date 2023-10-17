module;

#undef UNICODE
#include <windows.h>

export module graphics;

import <typeinfo>;
import "body.hpp";
import time;

static HWND g_Hwnd;

export unsigned int get_rgb(unsigned int r, unsigned int g, unsigned int b) 
{
    return r + (g << 8) + (b << 16);
}
 
// Draw functions

export void fill_rect(float x, float y, float sx, float sy, unsigned int color) {
    HDC hdc = GetDC(g_Hwnd);
    HBRUSH br = CreateSolidBrush(color);
    RECT rc;
    SetRect(&rc, static_cast<int>(x), static_cast<int>(y), static_cast<int>(x+sx), static_cast<int>(y+sy));
    FillRect(hdc, &rc, br);
    DeleteObject(br);
    ReleaseDC(g_Hwnd, hdc);
}

export void clear_screen() {
    fill_rect(0, 0, sim::screen_w, sim::screen_h, get_rgb(100, 100, 100));
}

export void frame_rect(float x, float y, float sx, float sy, unsigned int color) {
    HDC hdc = GetDC(g_Hwnd);
    HBRUSH br = CreateSolidBrush(color);
    RECT rc;
    SetRect(&rc, static_cast<int>(x), static_cast<int>(y), static_cast<int>(x + sx), static_cast<int>(y + sy));
    FrameRect(hdc, &rc, br);
    DeleteObject(br);
    ReleaseDC(g_Hwnd, hdc);
}

export void draw_line(float x1, float y1, float x2, float y2, unsigned int color) {
    HDC hdc = GetDC(g_Hwnd);
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HGDIOBJ old_pen = SelectObject(hdc, pen);
    MoveToEx(hdc, static_cast<int>(x1), static_cast<int>(y1), NULL);
    LineTo(hdc, static_cast<int>(x2), static_cast<int>(y2));
    SelectObject(hdc, old_pen);
    DeleteObject(pen);
    ReleaseDC(g_Hwnd, hdc);
}

export void draw_proj(const Projectile& a) 
{
    draw_line(a.m_c.x, a.m_c.y, a.m_c.x + a.m_hw * a.m_dir.x, a.m_c.y + a.m_hw * a.m_dir.y, get_rgb(255, 255, 255));
}

export void draw_clock(const Clock& a, t_time& t) {
    vec2f head_sec(a.m_hw, 0.f), head_min(a.m_hw / 1.25f, 0.f), head_hr(a.m_hw / 1.5f, 0.f);
    head_sec.transform(vec2f(a.m_c.x, a.m_c.y), math::pi / 2.f - t.sec * 2 * math::pi / 60.f);
    head_min.transform(vec2f(a.m_c.x, a.m_c.y), math::pi / 2.f - t.min * 2 * math::pi / 60.f);
    head_hr.transform(vec2f(a.m_c.x, a.m_c.y), math::pi / 2.f - t.hr * 2 * math::pi / 12.f);
    draw_line(a.m_c.x, a.m_c.y, head_sec.x, head_sec.y, get_rgb(0, 0, 0));
    draw_line(a.m_c.x, a.m_c.y, head_min.x, head_min.y, get_rgb(255, 0, 255));
    draw_line(a.m_c.x, a.m_c.y, head_hr.x, head_hr.y, get_rgb(255, 255, 0));
    frame_rect(a.m_c.x - a.m_hw, a.m_c.y - a.m_hw, 2.f * a.m_hw, 2.f * a.m_hw, get_rgb(127, 255, 212));
}

export void draw_cannon(float cannon_x, float cannon_y, float cannon_tilt) {
    vec2f a(0, -sim::cannon_h), b(-sim::cannon_w / 2, 0), c(sim::cannon_w / 2, 0);
    a.transform(vec2f(cannon_x, cannon_y), cannon_tilt);
    b.transform(vec2f(cannon_x, cannon_y), cannon_tilt);
    c.transform(vec2f(cannon_x, cannon_y), cannon_tilt);
    draw_line(a.x, a.y, b.x, b.y, get_rgb(0, 0, 0));
    draw_line(b.x, b.y, c.x, c.y, get_rgb(0, 0, 0));
    draw_line(c.x, c.y, a.x, a.y, get_rgb(0, 0, 0));
}

export void clear_body(Body& a) 
{
    fill_rect(a.m_c.x - a.m_hw , a.m_c.y - a.m_hw , 3 * a.m_hw, 3 * a.m_hw, get_rgb(0, 0, 0));
}

// Graphics library

export struct s_init {
    HINSTANCE   instance;
    WNDPROC     wndProc;
    const char* captionText;
    int         screen_width;
    int         screen_height;
};

export HWND InitializeGraphics(const s_init& init) {
    WNDCLASSEX wc;
    memset(&wc, 0, sizeof(wc));
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = init.wndProc;
    wc.hInstance     = init.instance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.lpszClassName = "MainWindow";
    RegisterClassEx(&wc);

    DWORD dw_style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
    RECT rc;
    SetRect(&rc, 0, 0, init.screen_width, init.screen_height);
    AdjustWindowRect(&rc, dw_style, false);
    rc.right  -= rc.left;
    rc.bottom -= rc.top;

    g_Hwnd = CreateWindowEx( 0,
        "MainWindow",
        init.captionText,
        dw_style | WS_VISIBLE,
        (GetSystemMetrics(SM_CXSCREEN)-rc.right)/2,
        (GetSystemMetrics(SM_CYSCREEN)-rc.bottom)/2,
        rc.right, rc.bottom,
        NULL,
        NULL,
        init.instance,
        NULL);

    return g_Hwnd;
}

export void UninitializeGraphics() 
{
    DestroyWindow(g_Hwnd);
}
