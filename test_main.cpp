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
    //RUN_TEST(default, "unknown");
    //RUN_GROUP(custom);
    //bwtest::setOutput("test.txt");// out put to file
    RUN_GROUP(sample1);
    RUN_TEST(custom, MyCustomTestClass);
    RUN_TEST(sample1, exceptions);
    //RUN_GROUP(sample2);
    TYPENAME_OUT std::vector<int>::iterator iter;
    return 0;
}
