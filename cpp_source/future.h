#ifndef Future_h_
#define Future_h_

#include <future>
#include <thread>
#include <iostream>
#include <deque>
#include <utility>
#include <mutex>




//future
int answer_to_everything()
{
    return 42;
}
void test_future()
{
    auto f = std::async(std::launch::async, answer_to_everything);
    std::cout << "the answer to everything is: " << f.get() << std::endl;
}

#endif