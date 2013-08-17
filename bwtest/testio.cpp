/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

 
#include "testio.h"

namespace bwtest
{

    void setOutput(const char * c_str) {
        if (!strncmp(c_str, "cout", 4)) {
            outputStream = stdCout;
        }
        else if (!strncmp(c_str, "cerr", 4)) {
            outputStream = stdCerr;
        }
        else if (!strncmp(c_str, "clog", 4)) {
            outputStream = stdClog;
        }
        else {
            g_fileName = const_cast<char*>(c_str);
            outputStream = file;
        }
    }

     std::ostream& getOutputStream() {
        switch(outputStream) {
        case stdCout : return std::cout;
        case stdCerr : return std::cerr;
        case stdClog : return std::clog;
        case file :
            static std::ofstream bwOStream(g_fileName, std::ios::app);
            if (bwOStream.is_open())
                return bwOStream;
            else {
                throw std::runtime_error("cannot open file");
            }
        } // switch
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
    

namespace BWTestInternal {

PrintAux:: PrintAux(
            bool statement,
            bool expectation,
            const char* expectText0,
            const char* expectText1,
            const char* actualText,
            const char* filePath,
            int lineNumber,
            const char* func_name)

        : expect_failed_(statement != expectation) {

    if(expect_failed_) {
       bwtest::getOutputStream()
            << "\n\n>>>  " << filePath
            << "\n>>>  Line: " << lineNumber << "\t function name: " << func_name
            << "\n>>>  Expect: "
            << expectText0 << expectText1
            << "\n>>>  Actual: " << actualText
            << std::endl;
    }
}

bool PrintAux::has_failed() const BW_NOEXCEPT {
    return expect_failed_;
}


bool PrintAux::BWTEST_bool_caughtExcepttion = false;


}
} // namespace bwtest











