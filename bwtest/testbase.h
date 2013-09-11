/*
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#ifndef _BOWEN_TESTBASE_H
#define _BOWEN_TESTBASE_H


#include "config.h"
#include "testio.h"
#include "benchmark.h"

namespace bwtest {


const char* DEFALUT_GROUP  ="default";
const char* CUSTOM_GROUP = "custom";

    //virtual base class
class TestBase
{
private: // internal fields
        const char* _M_testgroup;
        const char* _M_testcase;
        size_t _M_excution_times;
        size_t _M_excution_num;

protected:
        bwtest::Benchmark benchmark; // for client code

public:
        BWTEST_NO_ASSIGN(TestBase);
        BWTEST_NO_COPY(TestBase);
        explicit TestBase(const char* testcase)
        :   _M_testgroup(CUSTOM_GROUP),
            _M_testcase(testcase),
            _M_excution_times(1),
            _M_excution_num(0),
            benchmark(CountThreadTime) {}

        explicit TestBase(size_t tmies, const char* testcase, const char* testgroup = DEFALUT_GROUP)
        :   _M_testgroup(testgroup),
            _M_testcase(testcase),
            _M_excution_times(tmies),
            _M_excution_num(0),
            benchmark(CountThreadTime) {}

        virtual ~TestBase() BW_NOEXCEPT   {}
        
        virtual void run() = 0;

        const char* getTestCaseName() const  BW_NOEXCEPT {
            return _M_testcase;
        }
        const char* getTestGroupName() const BW_NOEXCEPT {
            return _M_testgroup;
        }
        void setExcuTime(size_t t) {
            this->_M_excution_times = t;
        }
		void excute() {
			for(size_t i = 0; i != _M_excution_times; ++i) {
				try {
					run();
				} catch(std::exception& e) {
					_M_excution_num++;
                    bwtest::getOutputStream() << "\n>>>  " << _M_testcase
					<< "    caught std exception:\n>>> " << e.what();
				} catch(...) {
					_M_excution_num++;
                    bwtest::getOutputStream() << "\n>>>  " << _M_testcase
					<< "    caught unknown exception";
				}
			}
		}

        void printReport() const BW_NOEXCEPT {
            
          bwtest::getOutputStream()
            << "\n    <Test "
            << " group=\""
                    << std::setw(10) << _M_testgroup << '\"' 
            << " case=\"" 
                    << std::setw(10) << _M_testcase << '\"'
            << " excution=\"" 
                    << std::setw(2) << _M_excution_times << '\"'
            << " exceptions=\""
                    << std::setw(2)  << _M_excution_num << '\"'
            << " time=\"" 
                    << std::setw(10) << (this->benchmark.getDuration()) << " micsec\""
            << ">";
        }
};


}
#endif // _BOWEN_TESTBASE_H
