#ifndef Promise_h_
#define Promise_h_

#include <functional>  // std::ref
#include <future>      // std::promise, std::future
#include <iostream>    // std::cout
#include <thread>      // std::thread

void print_int(std::future<int> &fut) {
    int x = fut.get();
    std::cout << "value: " << x << '\n';
}

void test_promise() {
    std::promise<int> prom;  // create promise

    std::future<int> fut = prom.get_future();  // engagement with future

    std::thread th1(print_int, std::ref(fut));  // send future to new thread

    prom.set_value(10);  // fulfill promise

    th1.join();
}

#endif