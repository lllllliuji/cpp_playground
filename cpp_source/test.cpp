#include "test.h"


int main() {
    simple_thread_pool pool;
    std::mutex mut;
    auto func = [&]() {
        auto id = std::this_thread::get_id();
        auto start = std::chrono::steady_clock::now();
        int cnt = 10;
        for (int i = 0; i < 1000000; i++) {
            cnt = cnt * 10 % 100000;
        }
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> cost = end - start;
        {
            std::lock_guard<std::mutex> lk(mut);
            std::cout << id << " runs for " << cost.count() << "s" << std::endl;
        }
    };

    for (int i = 0; i < 20; i++) {
        pool.submit(func);
    }
    long long cnt = 0;
    for (;;) {
        cnt ++;
    }
    return 0;
}

/*
    std::_List_const_iterator<std::pair<std::__cxx11::basic_string<char>, int> >
   const std::__cxx11::list<std::pair<std::__cxx11::basic_string<char>, int>,
   std::allocator<std::pair<std::__cxx11::basic_string<char>, int> > >::iterator

*/