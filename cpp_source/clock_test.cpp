#include "base.h"

int main() {
    auto now = std::chrono::system_clock::now();
    std::cout << "hello world" << std::endl;
    auto end = std::chrono::system_clock::now();

    std::chrono::nanoseconds x = end - now;
    std::cout << x.count() << std::endl;
    int64_t t = now.time_since_epoch().count();
    std::cout << t << std::endl;
    std::cout << INT64_MAX << std::endl;
    std::cout << INT64_MIN << std::endl;
    return 0;
}