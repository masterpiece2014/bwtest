/*
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#ifndef BWTESTIO_H
#define BWTESTIO_H



#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

#include "config.h"
#include "nullostream.h"
 
namespace bwtest
{

    enum BWTestOutputType{
        stdCout,
        stdCerr,
        stdClog,
        nullOS,
        file
    };

    static BWTestOutputType outputStream = stdCerr;
    static char* g_fileName  = BW_NULL_PTR;
    static bwtest::BWTestInternal::NullOStream NULL_OS;
    
    void setOutputStream(const char* c_str) {
        if (!strncmp(c_str, "cout", 4)) {
            outputStream = stdCout;
        }
        else if (!strncmp(c_str, "cerr", 4)) {
            outputStream = stdCerr;
        }
        else if (!strncmp(c_str, "clog", 4)) {
            outputStream = stdClog;
        }
        else if (!strncmp(c_str, "null", 4)){
            outputStream = nullOS;
        } else {
            g_fileName = const_cast<char*>(c_str);
            outputStream = file;
        }
    }
    
    std::ostream& getOutputStream() {
        switch(outputStream) {
        case stdCout :
                return std::cout;
        case stdCerr :
                return std::cerr;
        case stdClog : 
                return std::clog;
        case nullOS :
                return NULL_OS;
        case file :
            static std::ofstream bwOStream(g_fileName, std::ios::app);
            if (bwOStream.is_open()) {
                return bwOStream;
            }
            else {
                throw std::runtime_error("cannot open file");
            }
        } // switch
    }

    std::ostream& getNullOutputStream()  {
        return NULL_OS;
    }

    void cleanOutputStream() {
        if (file == outputStream) {
            static_cast<std::ofstream&>(getOutputStream()
            ).close();
        }
    }

    //// multi-type, multi-arguments printing
    ////    e.g.
    //// print(std::string("sdfds"), 32234, 546.546, "sdgdf", 'c');
    ////
#ifdef BWTEST_HAS_CXX11_
    void print(){}
    template<typename T, typename... Ts>
    void print(const T& obj, const Ts&... others) {
        std::cout << obj << " ";
        print(others...);
    }
#endif


} // namespace bwtest


#endif // BWTESTIO_H
