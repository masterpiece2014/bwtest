/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#include "bwtest.h"

#include <cstdio>
#include <stdarg.h>

namespace bwtest {

namespace BWTestInternal {
        inline void formatAux(char* ptr, int size, const char* format, ...) {
            va_list args;
            va_start(args, format);
            vsnprintf(ptr, size, format, args);
            va_end(args);
        }
}
        
        std::string toString(long long val) {
            char buf[4*sizeof(long long)];
            BWTestInternal::formatAux(buf, 4*sizeof(long long), "%lld", val);
            return std::string(buf);
        }
        std::string toString(unsigned long long val) {
            char buf[4*sizeof(unsigned long long)];
            BWTestInternal::formatAux(buf, 4*sizeof(unsigned long long), "%llu", val);
            return std::string(buf);
        }
        std::string toString(float val) {
//            float: max exponent10 38
            char buf[48];
            BWTestInternal::formatAux(buf, 48, "%f", val);
            return std::string(buf);
        }



namespace BWTestInternal {

        std::string makeErrorMsg(const char* file, const char* function, long long line) {
            return std::string("\n>>>  ").append(file)
                        .append("\n>>>  Function: ").append(function)
                        .append("\t Line: ").append(toString(line));
        }

        bool TestRegister::initialized = false;

        TestRegister::TestRegister()  {}

        TestRegister* TestRegister::instance() {
            if(TestRegister::initialized == false) {
                put_out << "<bwtest version=\"" << BWTEST_VERSION << "\">\n";
                initialized = true;
            }
            static TestRegister reg;
            return &reg;
        }
        TestRegister::~TestRegister() BW_NOEXCEPT {
            put_out << "\n</bwtest>\n\n";
            for (GroupMap::iterator i = tests_.begin(); i != tests_.end(); ++i) {
                for (size_t j = 0; j != i->second.size(); ++j) {
                    delete i->second.at(j);
                }
            }
            ::bwtest::cleanOutputStream();
        }
/////////////////////////////////////////////////////////////////////////////////
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

        int TestRegister::runGroup(const char* group) {
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

        int TestRegister::reportTest(const char* group, const char* name) BW_NOEXCEPT {
            TYPENAME GroupMap::iterator  grp = TestRegister::instance()->tests_.find(group);
            if (grp != TestRegister::instance()->tests_.end()) {
                for (TYPENAME Group::iterator i = grp->second.begin();
                                    i != grp->second.end();
                                    ++i) {
                    const char* testcase = (*i)->getTestCaseName();
                    if (0 == strcmp(name, testcase)) {
                        (*i)->printReport();
                        return 0;
                    }
                }
            }
            bwtest::getOutputStream() << "\n<RunTestCase "
            << std::setw(10) << " group=\"" << group << '\"' 
            << std::setw(10) << " case=\"" << name << '\"'
            << '>'
            << "\nNo such test case"
            << "\n</RunTestCase>";  
            return 0;
        }
        int TestRegister::reportGroup(const char* group) BW_NOEXCEPT {
                        GroupMap::iterator  grp = TestRegister::instance()->tests_.find(group);
            if (grp != TestRegister::instance()->tests_.end()) {
                bwtest::getOutputStream() 
                    << "\n<RunTestGroup"
                    << std::setw(10) << " group=\"" << group << '\"' 
                    << '>';
                for (size_t j = 0; j != grp->second.size(); ++j) {
                    grp->second.at(j)->printReport();
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

}// namespace BWTestInternal

} // bwtest
