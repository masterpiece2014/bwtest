/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#include "commen.h"

namespace bwtest {


enum CountingType {
        CountRealTime = 1,
        CountThreadTime
    };

struct Time {
    long sec;
    long mic_sec; // micro-second
};
typedef struct Time Time;


#ifdef BWTEST_OS_LINUX
#include <ctime>

Time getRealTime() {
    timespec start;
    Time mytime;
    assert(0 == 
        clock_gettime(CLOCK_REALTIME, &start)
    );
    mytime.sec = start.tv_sec;
    mytime.mic_sec = start.tv_nsec / 1000L;
    return mytime;
}

Time getThreadTime() {
    timespec start;
    Time mytime;
    assert(0 == 
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start)
    );
    mytime.sec = start.tv_sec;
    mytime.mic_sec = start.tv_nsec / 1000L;
    return mytime;
}

#endif

#ifdef BWTEST_OS_WIN32
#include <windows.h>


//http://www.drdobbs.com/windows/win32-performance-measurement-options/184416651

//http://msdn.microsoft.com/en-us/library/ms683237%28VS.85%29.aspx
//http://msdn.microsoft.com/en-us/library/ms683223%28VS.85%29.aspx


LARGE_INTEGER getFILETIMEoffset() {
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}

int win32_clock_gettime(int X, struct timeval *tv) {
    LARGE_INTEGER           t;
    FILETIME                fileTime;
    double                  microseconds;
    static LARGE_INTEGER    offset;
    static double           frequencyToMicroseconds;
    static int              initialized = 0;
    static BOOL             usePerformanceCounter = 0;

    if (!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
//        QueryPerformanceFrequency
//        作用：返回硬件支持的高精度计数器的频率。
//        返回值：非零，硬件支持高精度计数器；零，硬件不支持，读取失败。

        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&fileTime);
        t.QuadPart = fileTime.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= fileTime.dwLowDateTime;
    }
    //  warning C4244: '=' : conversion from 'bwtest::LONGLONG' to 'long', possible loss of data
    //timeutil.h(91): warning C4100: 'X' : unreferenced formal parameter
//): warning C4365: 'argument' : conversion from 'long' to 'unsigned int', signed/unsigned mismatch
    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = microseconds;
    tv->tv_sec = t.QuadPart / 1000000;
    tv->tv_usec = t.QuadPart % 1000000;
    return (0);
}


//tv_sec
//tv_usec

Time getRealTime() {
    timeval start;
    Time mytime;
    assert(0 == 
        win32_clock_gettime(0, &start)
    );
    mytime.sec = start.tv_sec;
    mytime.mic_sec = start.tv_usec;
    return mytime;
}

Time getThreadTime() {
    timeval start;
    Time mytime;
    assert(0 == 
        win32_clock_gettime(3, &start)
    );
    mytime.sec = start.tv_sec;
    mytime.mic_sec = start.tv_usec;
    return mytime;
}

#endif


}




