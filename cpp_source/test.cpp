#include "test.h"

std::mutex a;
std::mutex b;
int x = 0;

void f() {
    std::unique_lock<std::mutex> a_lock(a, std::defer_lock);
    std::unique_lock<std::mutex> b_lock(b, std::defer_lock);
    std::lock(a_lock, b_lock);
    x++;
}
void g() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::scoped_lock<std::mutex> lk(a);
    std::cout << x << std::endl;
}
int main() {
    std::thread t1(f);
    std::thread t2(g);
    t1.join();
    t2.join();
    std::list<int> li{1, 2, 3};
    li.remove_if([](const int &x) -> bool { return x <= 2; });
    std::for_each(li.begin(), li.end(), [](const int &x) { std::cout << x << std::endl; });
    li.pop_front();
    std::for_each(li.begin(), li.end(), [](const int &x) { std::cout << x << std::endl; });
    return 0;
}