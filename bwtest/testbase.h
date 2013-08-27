/*
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#ifndef _BOWEN_TESTBASE_H
#define _BOWEN_TESTBASE_H


#include "config.h"
#include "testio.h"
#include "benchmark.h"
#include <typeinfo>

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
            benchmark(CountThreadTime) {}

        explicit TestBase(size_t tmies, const char* testcase, const char* testgroup = DEFALUT_GROUP)
        :   __name_testgroup(testgroup),
            __name_testcase_(testcase),
            __num_excutions_(tmies),
            __num_except_(0),
            benchmark(CountThreadTime) {}

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
			for(size_t i = 0; i != __num_excutions_; ++i) {
				try {
                    // benchmark.start();
					run();
                    // benchmark.stop();
				} catch(std::exception& e) {
					__num_except_++;
                    bwtest::getOutputStream() << "\n>>>  " << __name_testcase_
					<< "    caught std exception: " << typeid(e).name()
                    << "\n>>> " << e.what();
				} catch(...) {
					__num_except_++;
                    bwtest::getOutputStream() << "\n>>>  " << __name_testcase_
					<< "    caught unknown exception";
				}
			}
		}

        void printReport() const BW_NOEXCEPT {
            
          bwtest::getOutputStream()
            << "\n    <Test "
            << " group=\""
                    << std::setw(10) << __name_testgroup << '\"' 
            << " case=\"" 
                    << std::setw(10) << __name_testcase_ << '\"'
            << " excution=\"" 
                    << std::setw(2) << __num_excutions_ << '\"'
            << " exceptions=\""
                    << std::setw(2)  << __num_except_ << '\"'
            << " time=\"" 
                    << std::setw(10) << (this->benchmark.getDuration()) << " micsec\""
            << ">";
        }
};


}
#endif // _BOWEN_TESTBASE_H
