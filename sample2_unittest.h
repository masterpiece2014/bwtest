/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#include "sample2.h"

TEST(sample2, constructor, 1) {
    sample2 sp2;
}

#include <algorithm>
#include <vector>
void consumes() {
    std::vector<short> vec(1<<24);
    std::sort(vec.begin(), vec.end());
    std::is_sorted(vec.begin(), vec.end());
}

// optimazation disabled
TEST(win_time, t1, 1) {
  //  using namespace bwtest;
 //   Time t1 = getRealTime();
    benchmark.start();
    consumes();
    benchmark.stop();
   // put_out << benchmark.getDuration() << '\n';
   // Time t2 = getRealTime();
  //  put_out << "stressful env 4: "<< Benchmark::timeDiff(t2, t1) << " micro second \n";
}

