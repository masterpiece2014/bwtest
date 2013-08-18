/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

/*
 *  How to use bwtest
 */

// you file
#include "sample1.h"

//  test gruop      test case name      excution number
//      |               |                   |
TEST(sample1,       odd_even,               1) { 
    // group sample1, test odd_even will be excuted 1 time

    expect_true(true == isOdd(5)) << "if 5 is not odd, \
                                      hence this expectation failed, \
                                      this line will be put out.";

    expect_false(true == isOdd(7)) << "e.g. this line won't be put out\
                                        'cause 7 is odd";

    expect_eq(true, isOdd(11)) << "this line won't be put out";
    expect_nq(true, isOdd(10)) << "either";
}


// custom test class.
//  step 1 : inherite
class MyCustomTestClass : public bwtest::TestBase
{
    int mydata;
    // step 2 : define constructor
    CONSTRUCT(MyCustomTestClass) {
        // you initialzation code here
        this->mydata = 0;
    }

    // step 3 : overwrite run()
    void run() {
        put_out << "\n\t====test invoked====\n";
        long num = rand() / 10;
        expect_eq(myCorrectFactorialFunction(num), factorial(num)) << " wrong factorial"
                                                    << "correct: " << myCorrectFactorialFunction(num)
                                                    << "incrrect:" << factorial(num);

        expect_true(isPrime(97)) <<  97 << "is not prime";
        
        expect_true(isPrime(2));
        expect_true(isPrime(3));
        expect_true(isPrime(5));

        expect_false(isPrime(1));
        expect_false(isPrime(91));
        expect_false(isPrime(-1));
        expect_false(isPrime(-17));
    }

    int myCorrectFactorialFunction(long n) {
        int result = 1;
        for (long i = 1; i <= n; i++) {
            result *= i;
        }
        return result;
    }
};
// step 4: register
REGISTER(MyCustomTestClass);

//      test exception
TEST(sample1, exceptions, 2) {
    put_out << "\n\n===this test will be excuted twice";

    expect_throw(throwRangeError(), std::range_error);
    expect_throw(throwRangeError(), std::bad_alloc) << "caught no bad_alloc";

    expect_throw_any(throwRandomStdExcept());
}

