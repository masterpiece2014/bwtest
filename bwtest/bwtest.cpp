/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#include "bwtest.h"

#include <cstdio>
#include <stdarg.h>

namespace bwtest {
namespace BWTestInternal
{
        inline void formatAux(char* ptr, int size, const char* format, ...) {
            va_list args;
            va_start(args, format);
            vsnprintf(ptr, size, format, args);
            va_end(args);
        }
        
        std::string toString(long long val) {
            char buf[4*sizeof(long long)];
            formatAux(buf, 4*sizeof(long long), "%lld", val);
            return std::string(buf);
        }
        std::string toString(unsigned long long val) {
            char buf[4*sizeof(unsigned long long)];
            formatAux(buf, 4*sizeof(unsigned long long), "%llu", val);
            return std::string(buf);
        }
        std::string toString(float val) {
//            float: max exponent10 38
            char buf[48];
            formatAux(buf, 48, "%f", val);
            return std::string(buf);
        }

        std::string makeErrorMsg(const char* file, const char* function, long long line) {
            return std::string("\n>>>  ").append(file)
                        .append("\n>>>  Function: ").append(function)
                        .append("\t Line: ").append(BWTestInternal::toString(line));
        }

        
        
        TestRegister* TestRegister::class_handler = BW_NULL_PTR;

        TestRegister* TestRegister::instance(){
            if(TestRegister::class_handler == BW_NULL_PTR) {
                class_handler = new TestRegister();
            }
            return class_handler;
        }


        bool TestRegister::registerTestDefaultGroup(::bwtest::TestBase* newTest) {
            TestRegister::instance()->tests_[DEFALUT_GROUP].push_back(newTest);
            return true;
        }

        bool TestRegister::registerTest(::bwtest::TestBase* newTest, const char* group) {
            TestRegister::instance()->tests_[group].push_back(newTest);
            return true;
        }

        int  TestRegister::runGroup(const char* group) {
            GroupMap::iterator  grp = TestRegister::instance()->tests_.find(group);
            if (grp != TestRegister::instance()->tests_.end()) {
                for (size_t j = 0; j != grp->second.size(); ++j) {
                    grp->second.at(j)->excute();
                }
                return 0;
            } else {
                put_out << ">>>  empty group: " << group << '\n';
                return 1;
            }
        }

        int TestRegister::runAllTests()  {
            TestRegister* obj = TestRegister::instance();
            for (TYPENAME_OUT GroupMap::iterator i = obj->tests_.begin();
                                        i != obj->tests_.end();
                                        ++i) {
                for (size_t j = 0; j != i->second.size(); ++j) {
                    i->second.at(j)->excute();
                }
            }
            return 0;
        }

        int TestRegister::runTest(const char* g, const char* name) {
            TYPENAME_OUT GroupMap::iterator  grp = TestRegister::instance()->tests_.find(g);
            if (grp != TestRegister::instance()->tests_.end()) {
                for (TYPENAME_OUT Group::iterator i = grp->second.begin();
                                    i != grp->second.end();
                                                        ++i) {
                    if (0 == strcmp(name, (*i)->getTestCaseName())) {
                        (*i)->excute();
                        break;
                    }
                }
                return 0;
            } else {
                put_out << ">>>  empty group: " << g << '\n';
                return 1;
            }
        }

//        void TestRegister::reportAllTests() BW_NOEXCEPT {
//                        ::bwtest::getOutputStream() << "\n\n=============================================\n";
//            ::bwtest::getOutputStream() << ">>>\t TEST REPORT" << std::endl;
//            time_t t_now = std::time(BW_NULL_PTR);
//            ::bwtest::getOutputStream() << ">>>  time: " << std::asctime(std::localtime(&t_now));
//            for(size_t i = 0; i != TestRegister::instance()->class_handler->list_testcases_.size(); ++i) {
//
//                ::bwtest::getOutputStream() << "--------------------------------------";
//                class_handler->list_testcases_[i]->printReport();
//            }
//
//            ::bwtest::getOutputStream() << "\n>>> Time elapsed: "
//                << class_handler->time_total_ << " seconds" << std::endl;
//
//            ::bwtest::getOutputStream() << "=============================================\n";
//        }


/////////////////////////////////////////////////////////////////////////////////
        TestRegister::~TestRegister() BW_NOEXCEPT {
            for (GroupMap::iterator i = tests_.begin(); i != tests_.end(); ++i) {
                for (size_t j = 0; j != i->second.size(); ++j) {
                    delete i->second.at(j);
                }
            }
            ::bwtest::cleanOutputStream();
        }
}// namespace BWTestInternal
}
