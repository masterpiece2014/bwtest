/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */


#include <stdexcept>
#include <cstdlib>

int isOdd(long i) {
    return 0 != i % 2; 
}

int isEven(long i) {
    return 0 == i % 2;
}

void throwRandomStdExcept() {
    int type = rand() % 4;
    switch(type) {
    case 0:
        throw std::bad_alloc();
    case 1:
        throw std::range_error("sample1");
    case 2:
        throw std::logic_error("sample1");
    case 3:
        throw std::overflow_error("sample1");
    }
}

void throwRangeError() {
    throw std::range_error("throwRangeError():");
}

long factorial(long n) {
  int result = 1;
  for (long i = 1; i <= n; i++) {
    result *= i;
  }
  return result;
}

bool isPrime(long n) {
    if (n <= 1) 
        return false;
    if (n % 2 == 0) 
        return n == 2;
    for (long i = 3; i*i < n; i += 2) {
        if (n % i == 0) 
            return false;
    }
    return true;
}
