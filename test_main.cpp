/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#include "bwtest\bwtest.h"
#include "bwtest\bwtest.cpp"

#include "sample1_unittest.h"
#include "sample2_unittest.h"

int main() {
    
    //bwtest::setOutputStream("test.xml");// out put to file
    //RUN_TEST(default, "unknown");
   // RUN_GROUP(custom);
    //RUN_GROUP(sample1);
    //RUN_TEST(custom, MyCustomTestClass);
    //RUN_TEST(sample1, exceptions);
    //RUN_GROUP(sample2);
    
    RUN_GROUP(win_time);
   // RUN_TEST(win_time, t1);
    REPORT_ALL();
    return 0;
}
