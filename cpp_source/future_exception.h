#ifndef Future_exception_h_
#define Future_exception_h_

#include <iostream>
#include <future>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <math.h>

/*
    if the function call invoked as part of std::async
    throws an exception, that exception is stored in the future in place of a stored value, the
    future becomes ready, and a call to get() rethrows that stored exception.
*/

double square_root(double x) {
    if (x < 0) {
        throw std::out_of_range("x < 0");
    }
    return  std::sqrt(x);
}


void test_future_exception() {
    std::promise<double> p;
    std::future<double> pf = p.get_future();
    try {
        p.set_value(square_root(-1));
    }
    catch(...) {
        // p.set_exception(std::current_exception());
        p.set_exception(std::make_exception_ptr(std::logic_error("square_root")));
    }
    pf.get();
    // std::future<double> f = std::async(std::launch::async, square_root, -1);
    // double y = f.get();
}
#endif