/*
 *	Copyright  Bowen Cai
 *	MIT lecense
 * you can find the license on the bottom of this file
 */

#ifndef _BOWEN_TEST_H_
#define _BOWEN_TEST_H_

#include <stdexcept>
#include <map>
#include <string>
#include <vector>
#include <limits>

#include <cassert>
#include <cmath> // fabs
#include <cstring>
#include <iostream>

#include "config.h"
#include "testio.h"
#include "testbase.h"
 
//#include "bwtest.cpp"
namespace bwtest {
//  base of  all test classes
    class TestBase;

    // commander parse
    extern void init(int argc, char** argv);
    
    // 
    extern void setOutputStream(const char*);
    extern std::ostream& getOutputStream();
    extern std::ostream& getNullOutputStream();

    namespace BWTestInternal {
    
// All test classese get registered here.
        class TestRegister {
            private:
                TestRegister();

                BWTEST_NO_ASSIGN(TestRegister);
                BWTEST_NO_COPY(TestRegister);

            private:
                typedef std::vector<bwtest::TestBase*>          Group;
                typedef std::map<std::string, Group>            GroupMap;
                GroupMap tests_;
                
                struct FailureInfo;
                typedef std::vector<TestRegister::FailureInfo>  FailureGroup;
                FailureGroup    failures_;

            public:
            /// class_handler itself does not  need to be deleted
                ~TestRegister() BW_NOEXCEPT;
                
                bool registerTestDefaultGroup(::bwtest::TestBase* newTest);
                bool registerTest(::bwtest::TestBase* newTest, const char* group);
                bool registerFailure(const char*, const char*, int);
        
                int runTest(const char*group, const char*name);
                int runGroup(const char* group);
                int runAllTests();

                int reportTest(const char*group, const char*name) const;
                int reportGroup(const char* group) const;
                int reportAllTests() const;
                
                int reportFailures() const;
        
                static TestRegister* instance();
        };
        
        // print necessary info at expectation failure
        class PrintAux {
            #ifdef BWTEST_HAS_CXX11_
                PrintAux() = delete;
            #else
                PrintAux();
            #endif
                BWTEST_NO_COPY(PrintAux);
                BWTEST_NO_ASSIGN(PrintAux);

            public: // only one ctor is enabled
                explicit PrintAux(bool,bool,const char*,const char*,
                            const char*,const char*,int,const char*);
                
                bool expect_failed_;

                bool isFailed() const BW_NOEXCEPT;
                
                static bool BWTEST_bool_caughtExcepttion;
        };

// make std::string out of error file name, function name and line number
        extern std::string makeErrorMsg(const char*, const char*, long long);

    } // internal
}

template<typename PrintType>
std::ostream& operator<< (const bwtest::BWTestInternal::PrintAux& expAux,
                          const PrintType& msg) {
    if(expAux.isFailed()) {
        bwtest::getOutputStream() << ">>>  Message: " << msg;
        bwtest::getOutputStream() << std::endl;
        return bwtest::getOutputStream();
    }
    else {
        return bwtest::getNullOutputStream();
    }
}

namespace bwtest {

// utilities
extern std::string toString(long long);
extern std::string toString(unsigned long long);
extern std::string toString(float);

// for macro RANGE_CHECHK(value, target type)
template <typename Tgt, typename Src>
int rangeCheck(Src src) {
    Tgt t_max = std::numeric_limits<Tgt>::max();
    Tgt t_min = std::numeric_limits<Tgt>::min();
    if (src < t_min) {
        return -1;
    } else if (src > t_max) {
        return 1;
    } else {
        return 0;
    }
}

} // namespace bwtest

#undef put_out
#define put_out bwtest::getOutputStream()
#undef __MARK
#define __MARK put_out << "\n" << __FUNCTION__ << "\t" << __LINE__ << '\n'

///////////////////////////////////////////////////////////////////////////////

#undef _BWTEST_TEST_NAME_DFGRP
#define _BWTEST_TEST_NAME_DFGRP(test_name)\
    BWTestGroup_default__Case_##test_name##_End

#undef TESTCASE
#define TESTCASE(test_name, test_time)\
class _BWTEST_TEST_NAME_DFGRP(test_name) : public bwtest::TestBase\
{\
    void run();\
    public:\
        _BWTEST_TEST_NAME_DFGRP(test_name) ()\
        :   TestBase(test_time, #test_name)  {}\
};\
const bool BWTEST_##test_name##_registered =\
        BWTestInternal::TestRegister::instance()->registerTestDefaultGroup(\
                            new _BWTEST_TEST_NAME_DFGRP(test_name) ());\
void _BWTEST_TEST_NAME_DFGRP(test_name)::run()

/////////////////////////////////////
#undef _BWTEST_TEST_NAME
#define _BWTEST_TEST_NAME(test_group, test_name)\
    BWTestGroup_##test_group##__Case_##test_name##_End
///////////////////////////////////////////////////////////////////////////////
#undef TEST
#define TEST(test_group, test_name, test_time)\
class _BWTEST_TEST_NAME(test_group, test_name) : public bwtest::TestBase\
{\
    void run();\
    public:\
        _BWTEST_TEST_NAME(test_group, test_name) ()\
        :   TestBase(test_time, #test_name, #test_group)  {}\
};\
const bool BWTEST_##test_group##__##test_name##_registered =\
bwtest::BWTestInternal::TestRegister::instance()->registerTest(\
                new _BWTEST_TEST_NAME(test_group, test_name) (), #test_group);\
void _BWTEST_TEST_NAME(test_group, test_name)::run()

#undef CONSTRUCT
#define CONSTRUCT(CustomClass)\
    public:\
    CustomClass() : bwtest::TestBase(#CustomClass)

#undef REGISTER
#define REGISTER(CustomClass)\
const bool BWTEST_CUSTOM_##CustomClass##_registered =\
    bwtest::BWTestInternal::TestRegister::instance()->registerTest(\
                                    new CustomClass(), ::bwtest::CUSTOM_GROUP)
///////////////////////////////////////////////////////////////////////////////


#undef RANGE_CHEACK
#define RANGE_CHEACK(val, TypeName)\
{\
    int result = rangeCheck<TypeName>(val);\
    if (1 == result) {\
        throw std::overflow_error(\
                bwtest::makeErrorMsg(__FILE__,\
                                    __FUNCTION__, (long long)__LINE__)\
                .append("\n>>>  overflow in converting to "#TypeName).append("\n")\
          );\
    } else if (-1 == result) {\
        throw std::underflow_error(\
                bwtest::makeErrorMsg(__FILE__,\
                                    __FUNCTION__, (long long)__LINE__)\
                .append("\n>>>  underflow in converting to "#TypeName).append("\n")\
          );\
    }\
}

////////////////////////////////////////////////////////////
#undef RUN_ALL
#define RUN_ALL \
    bwtest::BWTestInternal::TestRegister::instance()->runAllTests

#undef RUN_GROUP
#define RUN_GROUP(group)\
    bwtest::BWTestInternal::TestRegister::instance()->runGroup(#group)

#undef RUN_TEST
#define RUN_TEST(group, name)\
        bwtest::BWTestInternal::TestRegister::instance()->runTest(#group, #name)
/////////////////////////////////////////////////////////
#undef REPORT_TEST
#define REPORT_TEST(group, name)\
        bwtest::BWTestInternal::TestRegister::instance()->reportTest(#group, #name)

#undef REPORT_GROUP
#define REPORT_GROUP(group)\
    bwtest::BWTestInternal::TestRegister::instance()->reportGroup(#group)

#undef REPORT_FAILURES
#define REPORT_FAILURES\
        bwtest::BWTestInternal::TestRegister::instance()->reportFailures
////////////////////////////////////////////////////////////
#undef REPORT_ALL
#define REPORT_ALL\
        bwtest::BWTestInternal::TestRegister::instance()->reportAllTests
////////////////////////////////////////////////////////////


#undef __MEM_FUNC__
#define __MEM_FUNC__\
    ((0 == strncmp("run", __FUNCTION__, 3)) ?           \
            this->getTestCaseName() :                   \
                            __FUNCTION__)

#undef expect_true
#define expect_true( statement )                                                \
        bwtest::BWTestInternal::PrintAux((statement),                           \
                    true,                                                       \
                  #statement,                                                   \
                  "  is true",                                                  \
                  "   is false",                                                \
                  __FILE__,                                                     \
                  __LINE__,                                                     \
                  __MEM_FUNC__)




//((0 == strncmp("run", __FUNCTION__, 3) ? )
//        this->getTestCaseName() :
//                            __FUNCTION__)




#undef expect_false
#define expect_false(statement)\
        bwtest::BWTestInternal::PrintAux((statement),\
                    false,\
                  #statement,\
                  "  is false",\
                  "  is true",\
                  __FILE__,\
                  __LINE__,\
                  __MEM_FUNC__)

#undef expect_eq
#define expect_eq(x, y)\
        bwtest::BWTestInternal::PrintAux( (x == y),\
                    true,\
                    #x,\
                    " == "#y,\
                    " not equal",\
                  __FILE__,\
                  __LINE__,\
                  __MEM_FUNC__)

#undef expect_eq_at
#define expect_eq_at(x, y, pres)\
        bwtest::BWTestInternal::PrintAux( (fabs(x - y) <= pres),\
                    true,\
                    #x,\
                    " == "#y,\
                    "not equal at "#pres,\
                  __FILE__,\
                  __LINE__,\
                  __MEM_FUNC__)

#undef expect_nq_at
#define expect_nq_at(x, y, pres)\
        bwtest::BWTestInternal::PrintAux( (fabs(x - y) > pres),\
                    true,\
                    #x,\
                    " != "#y,\
                    " equal at "#pres,\
                  __FILE__,\
                  __LINE__,\
                  __MEM_FUNC__)

#undef expect_nq
#define expect_nq(x, y)\
        bwtest::BWTestInternal::PrintAux( (x != y),\
                    true,\
                    #x,\
                    " != "#y,\
                    " equals ",\
                  __FILE__,\
                  __LINE__,\
                  __MEM_FUNC__)

#undef expect_throw
#define expect_throw(statement, exceptType)\
    bwtest::BWTestInternal::PrintAux::BWTEST_bool_caughtExcepttion = false;\
    try{statement;}\
    catch(exceptType e){bwtest::BWTestInternal::PrintAux::BWTEST_bool_caughtExcepttion = true;}\
    catch(...){}\
    bwtest::BWTestInternal::PrintAux(bwtest::BWTestInternal::PrintAux::BWTEST_bool_caughtExcepttion,\
                                    true,\
                                    #statement,\
                                    "  throws "#exceptType,\
                                    "  didn't throw "#exceptType,\
                                    __FILE__,\
                                    __LINE__,\
                                    __MEM_FUNC__)

#undef expect_throw_any
#define expect_throw_any(statement)\
    bwtest::BWTestInternal::PrintAux::BWTEST_bool_caughtExcepttion = false;\
    try{statement;}\
    catch(...){bwtest::BWTestInternal::PrintAux::BWTEST_bool_caughtExcepttion = true;}\
    bwtest::BWTestInternal::PrintAux(bwtest::BWTestInternal::PrintAux::BWTEST_bool_caughtExcepttion,\
                                    true,\
                                    #statement,\
                                    "  throws any thing",\
                                    "  throws nothing",\
                                    __FILE__,\
                                    __LINE__,\
                                    __MEM_FUNC__)
//////////////////////////////////////////////////////////////////////////////
#undef assert_eq
#define assert_eq(x, y)\
        assert(fabs(x - y) <= 0.00001);

#undef assert_nq
#define assert_nq(x, y)\
    assert(fabs(x - y) > 0.00001);

#undef assert_eq_at
#define assert_eq_at(x, y, z)\
    assert(fabs(x - y) < z);

#undef assert_throw
#define assert_throw(statement, expected_except)\
        {\
            bool caught = false;\
            try{statement;}\
            catch(expected_except const& e){caught = true;}\
            catch(...){}\
            assert(caught);\
        }

#undef assert_throw_any
#define assert_throw_any(statement)\
        {\
            bool caught = false;\
            try{statement;}\
            catch(...){caught = true;}\
            assert(caught);\
        }


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

//@deprecated
#undef LOOP_FUNC
#define LOOP_FUNC(func, loops)\
            {\
                size_t std_except_caught = 0;\
                size_t unknown_except_caught = 0;\
                for(size_t i = 0; i != loops; ++i ) {\
                    try{\
                        func();\
                    }catch(std::exception& e) {\
                        std_except_caught++;\
                        OUT_ONCE(BWTestInternal::makeErrorMsg(__FILE__, __FUNCTION__, (long long)__LINE__));\
                        OUT_ONCE("\n>>>  caught std exception\n");\
                        OUT_ONCE(e.what());\
                        OUT_ONCE("\n");\
                    }\
                    catch(...) {\
                        unknown_except_caught++;\
                        OUT_ONCE(BWTestInternal::makeErrorMsg(__FILE__, __FUNCTION__, (long long)__LINE__));\
                        OUT_ONCE("\n>>>  caught unknown exception");\
                        OUT_ONCE("\n");\
                    }\
                }\
                OUT_ONCE("\n>>  Totally caught: ");\
                OUT_ONCE(std_except_caught + unknown_except_caught);\
                OUT_ONCE("\n>>>    std exception : ");\
                OUT_ONCE(std_except_caught);\
                OUT_ONCE("\n>>>    unknown exception: ");\
                OUT_ONCE(unknown_except_caught);\
                OUT_ONCE("\n");\
            }

#undef OUT_ONCE

#endif //_BOWEN_TEST_H_



//     The MIT License (MIT)

// Copyright (c) 2013 Bowen Cai

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.




