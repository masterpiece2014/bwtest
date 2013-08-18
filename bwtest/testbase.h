/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#ifndef _BOWEN_TESTBASE_H
#define _BOWEN_TESTBASE_H


#include "commen.h"
#include "testio.h"
#include "benchmark.h"

namespace bwtest {


const char* DEFALUT_GROUP  ="default";
const char* CUSTOM_GROUP = "custom";

    //virtual base class
class TestBase
{
private: // internal fields
        const char* __name_testgroup;
        const char* __name_testcase_;
        size_t __num_excutions_;
        size_t __num_except_;

protected:
        bwtest::Benchmark benchmark; // for client code

public:
        BWTEST_NO_ASSIGN(TestBase);
        BWTEST_NO_COPY(TestBase);
        explicit TestBase(const char* testcase)
        :   __name_testgroup(CUSTOM_GROUP),
            __name_testcase_(testcase),
            __num_excutions_(1),
            __num_except_(0),
            benchmark(bwtest::CountThreadTime) {}

        explicit TestBase(size_t tmies, const char* testcase, const char* testgroup = DEFALUT_GROUP)
        :   __name_testgroup(testgroup),
            __name_testcase_(testcase),
            __num_excutions_(tmies),
            __num_except_(0),
            benchmark(bwtest::CountThreadTime) {}

        virtual ~TestBase() BW_NOEXCEPT   {}
        
        virtual void run() = 0;

        const char* getTestCaseName() const  BW_NOEXCEPT {
            return __name_testcase_;
        }
        const char* getTestGroupName() const BW_NOEXCEPT {
            return __name_testgroup;
        }
        void setExcuTime(size_t t) {
            this->__num_excutions_ = t;
        }
		void excute() {
			try {
				for(size_t i = 0; i != __num_excutions_; ++i) {
					try {
						run();
					} catch(std::exception& e) {
						__num_except_++;
                        OUT_ONCE("\n>>>");
						OUT_ONCE(__name_testcase_);
						OUT_ONCE("       caught std exception:\n>>> ");
						OUT_ONCE(e.what());
					} catch(...) {
						__num_except_++;
                        OUT_ONCE("\n>>>");
						OUT_ONCE(__name_testcase_);
						OUT_ONCE("       caught other exception");
					}
				}
			}
			catch(...) {
			    OUT_ONCE("\n>>> ");
			    OUT_ONCE(__name_testcase_);
                OUT_ONCE("  testBody: caught unknown exception. \n");
			}
		}

        void printReport() const BW_NOEXCEPT {
          bwtest::getOutputStream()
          << "\n    <Test "
          << " group=\"" << __name_testgroup << '\"' 
           << " case=\"" << __name_testcase_ << '\"'
           << " excution=\"" << __num_excutions_ << '\"'
          << " exceptions=\"" << __num_except_ << '\"'
           << " time=\"" << (this->benchmark.getDuration()) << " micsec\""
          << ">";
        }
};


}
#endif // _BOWEN_TESTBASE_H
