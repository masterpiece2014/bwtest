/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */


#include "bwtest/bwtest.h"
#include "bwtest/bwtest.cpp"

//#include "sample1_unittest.h"
//#include "sample2_unittest.h"

int main(int argc, char** argv) {
    //              0123456

    
    char* ptr1 = "this name";
    char* ptr = "--help";
    char* pptr[2];
    pptr[1] = ptr;
    pptr[2] = ptr1;
    ::bwtest::init(2, pptr);
    //bwtest::setOutputStream("test.xml");// out put to file
   // RUN_TEST(default, "unknown");
    //RUN_GROUP(sample1);
    //RUN_TEST(custom, MyCustomTestClass);
    //RUN_TEST(sample1, exceptions);
    //RUN_GROUP(sample2);
    
   // RUN_GROUP(custom);
   // RUN_GROUP(win_time);
//    REPORT_GROUP(win_time);
  //  REPORT_GROUP(custom);
//   RUN_TEST(win_time, t1);
  //  REPORT_ALL();
    return 0;
}
