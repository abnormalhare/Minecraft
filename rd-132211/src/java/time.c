#include "java/time.h"

s64 nanoTime(void) {
    LARGE_INTEGER freq, count;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&count);
    return count.QuadPart * 1000000000 / freq.QuadPart;
}

s64 currentTimeMillis(void) {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    s64 time = ((s64)ft.dwHighDateTime << 32) | ft.dwLowDateTime;

    return (time - 116444736000000000ULL) / 10000ULL;
}