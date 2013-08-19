/*
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#include <cassert>
#include "config.h"

enum CountingType {
        CountRealTime = 1,
        CountThreadTime
    };

struct Time {
    long sec;
    long mic_sec; // micro-second
};
typedef struct Time Time;


#ifdef BWTEST_SYS_POSIX
#include <time.h>
#include <unistd.h>

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

int getProcessorNumber() {    
    return sysconf(_SC_NPROCESSORS_ONLN);
}
#endif

#ifdef BWTEST_SYS_WIN

#include <windows.h>

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

int win32_clock_gettime(struct timeval *tv) {
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
//        返回值：非零，硬件支持高精度计数器；零，硬件不支持，读取失败
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter) {
        QueryPerformanceCounter(&t);
    }
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

Time getRealTime() {
    timeval start;
    Time mytime;
    assert(0 == 
        win32_clock_gettime(&start)
    );
    mytime.sec = start.tv_sec;
    mytime.mic_sec = start.tv_usec;
    return mytime;
}

Time getThreadTime() {
    Time mytime;
    LARGE_INTEGER total;
    FILETIME creation, exit, kernel, user;
    GetThreadTimes( GetCurrentThread() , &creation, &exit, &kernel, &user);
    total.HighPart = user.dwHighDateTime + kernel.dwHighDateTime;
    total.LowPart = user.dwLowDateTime + kernel.dwLowDateTime;
    mytime.sec = total.QuadPart / 1000000;
    mytime.mic_sec = total.QuadPart % 1000000;
    return mytime;
}


int getProcessorNumber() {
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    return siSysInfo.dwNumberOfProcessors;
}

#endif



#ifdef BWTEST_SYS_MAC

#include <mach/clock.h>
#include <mach/mach.h>
#include <sys/time.h>
#include <sys/param.h>
#include/ <sys/sysctl.h>

Time getRealTime() {
    timeval time;
    Time mytime;
    gettimeofday(&time, NULL);
    mytime.sec = time.tv_sec;
    mytime.mic_sec = time.tv_usec;
    return mytime;
}

Time getThreadTime() {
    Time mytime;
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    mytime.sec = mts.tv_sec;
    mytime.mic_sec = mts.tv_nsec / 1000L;
    return mytime;
}

int getProcessorNumber() {        
    int nm[2];
    size_t len = 4;
    uint32_t count;

    nm[0] = CTL_HW;
    nm[1] = HW_AVAILCPU; // HW_NCPU
    sysctl(nm, 2, &count, &len, NULL, 0);
    if(count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if(count < 1) {
            count = 1; 
        }
    }
    return count;
}
#endif

