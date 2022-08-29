#ifndef Clock_h_
#define Clock_h_
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>

std::condition_variable cv;
bool done;
std::mutex m;
bool wait_loop() {
    auto const timeout =
        std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
    std::unique_lock<std::mutex> lk(m);
    while (!done) {
        if (cv.wait_until(lk, timeout) == std::cv_status::timeout) break;
    }
    return done;
}

void test_clock() { std::cout << wait_loop() << std::endl; }
#endif