/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */



#ifndef BENCHMARK_H
#define BENCHMARK_H


#include "commen.h"
#include "timeutil.h"


namespace  bwtest {
            
class Benchmark
{
public:
    BWTEST_NO_ASSIGN(Benchmark);
    BWTEST_NO_COPY(Benchmark);
    

private:
    CountingType type_;
    Time start_;
    Time end_;

public:
    
    Benchmark(CountingType t)
        :   type_(t)    {
        this->refresh();
    }

    void start() {
        if (CountRealTime == this->type_) {
            this->start_ = getRealTime();
        } else {
            this->start_ = getThreadTime();
        }
    }
    
    void stop() {
        if (CountRealTime == this->type_) {
           this->end_ = getRealTime();
        } else {
           this->end_ = getThreadTime();
        }

    }
    Time getCurrentTime() const {
        return CountRealTime == this->type_ ? 
              getRealTime() :  getThreadTime();
    }

    long getDuration() const BW_NOEXCEPT {
        return timeDiff(this->end_, this->start_);
    }
    
    void refresh() BW_NOEXCEPT {
        this->start_.sec = 0;
        this->start_.mic_sec = 0;
        this->end_.sec = 0 ;
        this->end_.mic_sec = 0;
    }
    
    bool isFresh() const BW_NOEXCEPT {
        return 0 == start_.sec 
                && 0 == start_.mic_sec 
                && 0 == end_.sec 
                && 0 == end_.mic_sec;
    }

    long static timeDiff(Time end, Time start) BW_NOEXCEPT {
      if (end.sec == start.sec) {
        assert(end.mic_sec >= start.mic_sec);
        return end.mic_sec - start.mic_sec;
      }
//      assert(end.sec > start.sec &&
//             end.sec - start.sec <
//             std::numeric_limits<long>::max() / 1000000L);
      return (end.sec - start.sec) * 1000000L + end.mic_sec - start.mic_sec;
    }
    
};

}


#endif // BENCHMARK_H
