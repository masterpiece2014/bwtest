/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#ifndef BWTESTIO_H
#define BWTESTIO_H


#include "commen.h"

#include <iostream>
#include <fstream>

namespace bwtest
{

    enum BWTestOutputType{
        stdCout,
        stdCerr,
        stdClog,
        file
    };

    static BWTestOutputType outputStream = stdCerr;
    static char* g_fileName  = BW_NULL_PTR;


    void setOutput(const char* c_str);

    std::ostream& getOutputStream();

    void cleanOutputStream();

    //// multi-type, multi-arguments printing
    ////    e.g.
    //// print(std::string("sdfds"), 32234, 546.546, "sdgdf", 'c');
    ////
    #ifdef BWTEST_HAS_CXX11_
    void print();
    
    template<typename T, typename... Ts>
    void print(const T& obj, const Ts&... others);
    #endif
    
    
} // namespace bwtest



#define put_out bwtest::getOutputStream()
#define __MARK put_out << "\n" << __FUNCTION__ << "\t" << __LINE__ << '\n';


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////  enable printing //////////////////////////////
//////////////////      in function loops and testCase.excute() ///////////////
                                #define DO_PRINT
///////////////////////////////////////////////////////////////////////////////

#define OUT_ONCE(obj)

#ifdef DO_PRINT
#undef OUT_ONCE
#define OUT_ONCE(obj) bwtest::getOutputStream() << obj

#endif

namespace bwtest {

namespace  BWTestInternal 
{

class PrintAux
{
    bool expect_failed_;

#ifdef BWTEST_HAS_CXX11_
    PrintAux() = delete;
#else
        PrintAux();
#endif
        
    BWTEST_NO_COPY(PrintAux);
    BWTEST_NO_ASSIGN(PrintAux);

public: // only one ctor is enabled
    explicit PrintAux(
                bool statement,
                bool expectation,
                const char* expectText0,
                const char* expectText1,
                const char* actualText,
                const char* filePath,
                int lineNumber,
                const char* func_name);

    bool has_failed() const BW_NOEXCEPT;

    static bool BWTEST_bool_caughtExcepttion;

};

}
}// bwtest

#include "testio.cpp"


#endif // BWTESTIO_H
