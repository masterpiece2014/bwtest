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

        int TestRegister::runTest(const char* g, const char* name) {
            TYPENAME GroupMap::iterator  grp = TestRegister::instance()->tests_.find(g);
            if (grp != TestRegister::instance()->tests_.end()) {
                for (TYPENAME Group::iterator i = grp->second.begin();
                                    i != grp->second.end();
                                    ++i) {
                    const char* testcase = (*i)->getTestCaseName();
                    if (0 == strcmp(name, testcase)) {
                        bwtest::getOutputStream() << "\n<RunTestCase "
                        << std::setw(10) << " group=\"" << (*i)->getTestGroupName() << '\"' 
                        << std::setw(10) << " case=\"" << (*i)->getTestCaseName() << '\"'
                        << ">\n";

                        (*i)->excute();
                
                        bwtest::getOutputStream() << "\n</RunTestCase>\n";
                        
                        return 0;
                    }
                }
            }
            bwtest::getOutputStream() << "\n<RunTestCase "
            << std::setw(10) << " group=\"" << g << '\"' 
            << std::setw(10) << " case=\"" << name << '\"'
            << '>'
            << "\nNo such test case"
            << "\n</RunTestCase>";        
        }

        int  TestRegister::runGroup(const char* group) {
            GroupMap::iterator  grp = TestRegister::instance()->tests_.find(group);
            if (grp != TestRegister::instance()->tests_.end()) {
                bwtest::getOutputStream() 
                    << "\n<RunTestGroup"
                    << std::setw(10) << " group=\"" << group << '\"' 
                    << '>';

                for (size_t j = 0; j != grp->second.size(); ++j) {

                    bwtest::getOutputStream() << "\n    <RunTestCase "
                    << std::setw(10) << " case=\"" << grp->second.at(j)->getTestCaseName() << '\"'
                    << ">\n";

                    grp->second.at(j)->excute();
                
                    bwtest::getOutputStream() << "    </RunTestCase>";
                }

               bwtest::getOutputStream()
                    << "\n</RunTestGroup>\n";
                return 0;
            }
            bwtest::getOutputStream() << "\n<RunTestGroup "
            << std::setw(10) << " group=\"" << group << '\"'
            << '>'
            << "\nNo such test group"
            << "\n</RunTestGroup>\n";
        }

        int TestRegister::runAllTests()  {
            TestRegister* obj = TestRegister::instance();
            for (TYPENAME GroupMap::iterator i = obj->tests_.begin();
                                        i != obj->tests_.end();
                                        ++i) {
                bwtest::getOutputStream() 
                    << "\n<RunTestGroup"
                    << std::setw(10) << " group=\"" << i->first << '\"' 
                    << '>';
                for (size_t j = 0; j != i->second.size(); ++j) {
                    bwtest::getOutputStream() << "\n    <RunTestCase "
                    << std::setw(10) << " case=\"" << i->second.at(j)->getTestCaseName() << '\"'
                    << ">\n";

                    i->second.at(j)->excute();

                    bwtest::getOutputStream() << "    </RunTestCase>";
                }
                bwtest::getOutputStream()
                    << "\n</RunTestGroup>\n";
            }
            return 0;
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


        int TestRegister::reportTest(const char*group, const char*name) BW_NOEXCEPT {
            return 0;
        }
        int TestRegister::reportGroup(const char* group) BW_NOEXCEPT {
            return 0;
        }
        int TestRegister::reportAllTests() BW_NOEXCEPT {
                    TestRegister* obj = TestRegister::instance();
            for (TYPENAME GroupMap::iterator i = obj->tests_.begin();
                                        i != obj->tests_.end();
                                        ++i) {
                bwtest::getOutputStream() 
                    << "\n<TestGroup"
                    << std::setw(10) << " group=\"" << i->first << '\"' 
                    << '>';
                for (size_t j = 0; j != i->second.size(); ++j) {
                    i->second.at(j)->printReport();
                }
                bwtest::getOutputStream()
                    << "\n</TestGroup>\n";
            }
            return 0;
        }
        
  //          <testsuites tests="3" failures="0" disabled="0" errors="0" time="0.001" name="AllTests">  
  //<testsuite name="CHashTableTest" tests="3" failures="0" disabled="0" errors="0" time="0.001">  
  //  <testcase name="hashfunc" status="run" time="0.001" classname="CHashTableTest" />  
  //  <testcase name="addget" status="run" time="0" classname="CHashTableTest" />  
  //  <testcase name="delget" status="run" time="0" classname="CHashTableTest" />  
  //</testsuite>  
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
