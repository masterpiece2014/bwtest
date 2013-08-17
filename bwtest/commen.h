/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#ifndef COMMENFILES_H
#define COMMENFILES_H


#ifdef __GNUC__

    #if __GNUC__*100 + __GNUC_MINOR__*10 + __GNUC_PATCHLEVEL__ > 470
        #define BWTEST_HAS_CXX11_
    #endif
    #if __GNUC__*100 + __GNUC_MINOR__*10 + __GNUC_PATCHLEVEL__ < 400
        #error "please up data your complier to GCC 4.0.0 and above"
    #endif

    #define TYPENAME    typename

    #pragma GCC diagnostic ignored "-Wreturn-type"
    #pragma GCC diagnostic ignored "-Wcomment"
    #pragma GCC diagnostic ignored "-Wunused-variable"

#else
#ifdef _MSC_VER
    #if _MSC_VER > 1700
        #define BWTEST_HAS_CXX11_
    #endif
    #if _MSC_VER < 1300
        #error "please up data your complier to VC 7.0 and above"
    #endif

    #define TYPENAME


    #pragma warning(disable: 4003) //   not enough actual parameters for macro
    #pragma warning(disable: 4996) // 'vsnprintf': This function or variable may be unsafe. Consider using vsnprintf_s instead
    #pragma warning(disable: 4715) // 'bwtest::getOutputStream' : not all control paths return a value
//    #pragma warning(disable: 4625) // nullostream : copy constructor could not be generated because a base class copy constructor is inaccessible
//4626: 'bwtest::BWTestInternal::NullOStream' : assignment operator

#endif
#endif


#if (defined(__unix__) || defined(__unix) || defined(unix) || defined(__linux__))
    #define BWTEST_SYS_POSIX 1
#elif defined(_WIN32)
    #define BWTEST_SYS_WIN 1
#elif (defined(__APPLE__) && defined(__MACH__))
    #define BWTEST_SYS_MAC 1
#else
    #error "unknown OS."
#endif
//////////////////////////////////////////////////////////////////////////////////////////////
                  //  #undef BWTEST_HAS_CXX11_
//////////////////////////////////////////////////////////////////////////////////////////////


#undef BWTEST_NO_ASSIGN
#undef BWTEST_NO_COPY
#undef BW_NOEXCEPT
#undef BW_NULL_PTR

#ifdef BWTEST_HAS_CXX11_

    #define BW_NOEXCEPT noexcept
    #define BW_NULL_PTR nullptr

    #define BWTEST_NO_ASSIGN(type)\
            type& operator= (const type&) = delete;\
            type& operator= (type&& ) = delete

    #define BWTEST_NO_COPY(type)\
        type(const type&)  = delete;\
        type(type&&) = delete

#else
    #define BW_NOEXCEPT throw()
    #define BW_NULL_PTR 0

    #undef BWTEST_NO_ASSIGN
    #undef BWTEST_NO_COPY

    #define BWTEST_NO_ASSIGN(type)\
        type& operator= (const type&)

    #define BWTEST_NO_COPY(type)\
        type(const type&)

#endif


#endif // COMMENFILES_H
