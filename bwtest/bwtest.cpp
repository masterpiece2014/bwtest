/*
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#include "bwtest.h"
 
#include <cstdio> // toString
#include <stdarg.h>

#define BWTEST_HELP\
    "\n\
    \n--help\thelp content\
    \n\n--out\tset output stream\n\t\t--out=\"cout\"\
             \n\t\t--out=\"test_result.xml\"\
    \n\n--run\tset tests to run \n\t\t--run=\"group\"\
             \n\t\t--run=\"group/test\"\n\t\t--run --all\
    \n\n--report\t set tests to report\n\t\t--report\"group\"\
             \n\t\t--report=\"group/test\"\n\t\t--report --all\
    \n\n"

        
namespace bwtest {

//0123456789
//--out="cout"

//0123456789
//--run="group/case"
//0123456789
//--run -all

//01234567890
//--report="group/case"
//--report -all
            
    void init(int argc, char** argv) {
        for(int i = 1; i < argc; ++i) {
            if (BW_NULL_PTR != strstr(argv[i], "--help")) {
                put_out << BWTEST_HELP;
                break;
            }
            if (BW_NULL_PTR != strstr(argv[i], "--out")) {
                setOutputStream(argv[i] + 7);
            }
//            else if (BW_NULL_PTR != strstr(argv[i], "--run")) {
//                char* dir = argv[i] + 7;
//                char *idx = strchr(dir, DELIM);
//                if (0 == idx - dir) {
//                    if (BW_NULL_PTR != strstr(argv[i] + 6, "--all")) {
//                        RUN_ALL();
//                    } else {
//                        ;//bwtest::BWTestInternal::TestRegister::instance()->runGroup();
//                    }
//                } else {
//                    ;//bwtest::BWTestInternal::TestRegister::instance()->runTest(group, name)
//                }
//            } 
//            else if (BW_NULL_PTR != strstr(argv[i], "--report")) {
//                char* dir = argv[i] + 10;
//                char *idx = strchr(dir, DELIM);
//                if (0 == idx - dir) {
//                    if (BW_NULL_PTR != strstr(argv[i] + 9, "--all")) {
//                        REPORT_ALL();
//                    } else {
//                        ;//bwtest::BWTestInternal::TestRegister::instance()->reportGroup(xxx);
//                    }
//                } else {
//                    ;//bwtest::BWTestInternal::TestRegister::instance()->reportTest(group, name)
//                }
//            } 
//            else {
//                put_out << "  cannot parse command: " << argv[i] << "   try: --help\n";
//            }
        }
    }

namespace BWTestInternal {

        inline void formatAux(char* ptr, int size, const char* format, ...) {
            va_list args;
            va_start(args, format);
            vsnprintf(ptr, size, format, args);
            va_end(args);
        }
} //     namespace BWTestInternal
        
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


        TestRegister::TestRegister()  {}

        TestRegister* TestRegister::instance() {
            
            static TestRegister reg;
            static bool initialized = false;
            if(false == initialized) {
                bwtest::getOutputStream() << "<bwtest version=\"" << BWTEST_VERSION << "\">\n";
                bwtest::getOutputStream() << std::boolalpha;// << std::showpos;
                initialized = true;
            }
            return &reg;
        }
        TestRegister::~TestRegister() BW_NOEXCEPT {
            put_out << "\n</bwtest>\n\n";
            for (GroupMap::const_iterator i = tests_.begin(); i != tests_.end(); ++i) {
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
            TYPENAME GroupMap::const_iterator  grp = TestRegister::instance()->tests_.find(g);
            if (grp != TestRegister::instance()->tests_.end()) {
                for (TYPENAME Group::const_iterator i = grp->second.begin();
                                    i != grp->second.end();
                                    ++i) {
                    const char* testcase = (*i)->getTestCaseName();
                    if (0 == strcmp(name, testcase)) {
                        bwtest::getOutputStream() << "\n<Run-TestCase "
                        << " group=\"" << (*i)->getTestGroupName() << '\"' 
                        << " case=\"" << (*i)->getTestCaseName() << '\"'
                        << ">\n";

                        (*i)->excute();
                
                        bwtest::getOutputStream() << "\n</Run-TestCase>";
                        
                        return 0;
                    }
                }
            }
            bwtest::getOutputStream() << "\n<Run-TestCase "
            << " group=\"" << g << '\"' 
            << " case=\"" << name << '\"'
            << '>'
            << "\n\tNo such test case"
            << "\n</Run-TestCase>";        
        }

        int TestRegister::runGroup(const char* group) {
            GroupMap::const_iterator  grp = TestRegister::instance()->tests_.find(group);
            if (grp != TestRegister::instance()->tests_.end()) {
                bwtest::getOutputStream() 
                    << "\n<Run-TestGroup"
                    << " group=\"" << group << '\"' 
                    << '>';

                for (size_t j = 0; j != grp->second.size(); ++j) {

                    bwtest::getOutputStream() << "\n    <Run-TestCase "
                    << std::setw(10) << " case=\"" << grp->second.at(j)->getTestCaseName() << '\"'
                    << ">\n";

                    grp->second.at(j)->excute();
                
                    bwtest::getOutputStream() << "\n    </Run-TestCase>";
                }

               bwtest::getOutputStream()
                    << "\n</Run-TestGroup>\n";
                return 0;
            }
            bwtest::getOutputStream() << "\n<Run-TestGroup "
            << " group=\"" << group << '\"'
            << '>'
            << "\n\tNo such test group"
            << "\n</Run-TestGroup>\n";
        }

        int TestRegister::runAllTests()  {
            TestRegister* obj = TestRegister::instance();
            for (TYPENAME GroupMap::const_iterator i = obj->tests_.begin();
                                        i != obj->tests_.end();
                                        ++i) {
                bwtest::getOutputStream() 
                    << "\n\n<Run-TestGroup"
                    << " group=\"" << i->first << '\"' 
                    << '>';
                for (size_t j = 0; j != i->second.size(); ++j) {
                    
                    bwtest::getOutputStream() << "\n    <Run-TestCase "
                    << " case=\"" << i->second.at(j)->getTestCaseName() << '\"'
                    << ">\n";

                    i->second.at(j)->excute();

                    bwtest::getOutputStream() << "\n    </Run-TestCase>";
                }
                bwtest::getOutputStream()
                    << "\n</Run-TestGroup>\n";
            }
            return 0;
        }

        int TestRegister::reportTest(const char* group, const char* name) const{
            TYPENAME GroupMap::const_iterator  grp = TestRegister::instance()->tests_.find(group);
            if (grp != TestRegister::instance()->tests_.end()) {
                for (TYPENAME Group::const_iterator i = grp->second.begin();
                                    i != grp->second.end();
                                    ++i) {
                    const char* testcase = (*i)->getTestCaseName();
                    if (0 == strcmp(name, testcase)) {
                        (*i)->printReport();
                        return 0;
                    }
                }
            }
            bwtest::getOutputStream() << "\n<TestCase "
            << " group=\"" << group << '\"' 
            << " case=\"" << name << '\"'
            << '>'
            << "\n\tNo such test case"
            << "\n</TestCase>";  
            return 0;
        }

        int TestRegister::reportGroup(const char* group) const{
            
            GroupMap::const_iterator  grp = TestRegister::instance()->tests_.find(group);
            bwtest::getOutputStream() 
                << "\n<TestGroup"
                << " group=\"" << group << '\"' 
                << '>';
            if (grp != TestRegister::instance()->tests_.end()) {
                for (size_t j = 0; j != grp->second.size(); ++j) {
                    grp->second.at(j)->printReport();
                }
            } else {
               bwtest::getOutputStream() << "\n\tNo such test group";
            }
            bwtest::getOutputStream() << "\n</TestGroup>\n";
            return 0;
        }
            
        int TestRegister::reportAllTests() const {
            
            TestRegister* obj = TestRegister::instance();
            for (TYPENAME GroupMap::const_iterator i = obj->tests_.begin();
                                        i != obj->tests_.end();
                                        ++i) {
                bwtest::getOutputStream() 
                    << "\n<TestGroup"
                    << " group=\"" << i->first << '\"' 
                    << '>';
                for (size_t j = 0; j != i->second.size(); ++j) {
                    i->second.at(j)->printReport();
                }
                bwtest::getOutputStream()
                    << "\n</TestGroup>\n";
            }
            return 0;
        }

        struct TestRegister::FailureInfo {
            const char* file;
            const char* function;
            int lineNumber;
            
            FailureInfo(const char* _file,
                        const char* _function,
                        int _lineNumber)
            :   file(_file),
                function(_function),
                lineNumber(_lineNumber) {}
        };
        
        bool TestRegister::registerFailure(const char* fl, const char* func, int ln) {
            this->failures_.push_back(TestRegister::FailureInfo(fl, func, ln));
        }

        
        int TestRegister::reportFailures() const {
            TestRegister* obj = TestRegister::instance();
            bwtest::getOutputStream() << "=====" << obj->failures_.size() << "================\n\n";
            for (TYPENAME FailureGroup::const_iterator i = obj->failures_.begin();
                                        i != obj->failures_.end();
                                        ++i) {
                bwtest::getOutputStream() 
                    << "\n<Expectation-Failure\n"
                    << "    file=\"" << i->file << '\"'
                    << "\n    function=\"" << i->function << '\"'
                    << "\n    line=\"" << i->lineNumber << '\"'
                    << "    />";
            }
            return 0;
        }
        
        PrintAux::PrintAux(
                bool statement,
                bool expectation,
                const char* expectText0,
                const char* expectText1,
                const char* actualText,
                const char* filePath,
                int lineNumber,
                const char* funcName)
                : expect_failed_(statement != expectation) {

            if(expect_failed_) {
                TestRegister::instance()->registerFailure(filePath, funcName, lineNumber);
                bwtest::getOutputStream()
                << "\n\n>>>  " << filePath
                << "\n>>>  Line: " << lineNumber << "\t function name: " << funcName
                << "\n>>>  Expect: "
                << expectText0 << expectText1
                << "\n>>>  Actual: " << actualText
                << std::endl;
//                bwtest::BWTestInternal::TestRegister::instance()->
            }
        }
    
        bool PrintAux::isFailed() const BW_NOEXCEPT {
            return expect_failed_;
        }

        bool PrintAux::BWTEST_bool_caughtExcepttion = false;
    
    }// namespace BWTestInternal

} // bwtest
