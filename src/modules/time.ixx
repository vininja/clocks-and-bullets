module;

#include <windows.h>

export module time;

export struct t_time {
    int hr = 0, min = 0, sec = 0;
};

export void get_time(t_time& t) {
    SYSTEMTIME time;
    GetLocalTime(&time);
    t.hr = time.wHour;
    t.min = time.wMinute;
    t.sec = time.wSecond;
}