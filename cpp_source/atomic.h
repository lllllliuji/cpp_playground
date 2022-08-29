#ifndef Atomic_h_
#define Atomic_h_

#include "base.h"

void foo_order(int a, int b) { std::cout << a << " " << b << std::endl; }
int get_num() {
    static int i = 0;
    return ++i;
}

// 顺序一致
std::atomic<bool> x, y;
std::atomic<int> z;
void write_x() { x.store(true, std::memory_order_seq_cst); }
void write_y() { y.store(true, std::memory_order_seq_cst); }
void read_x_then_y() {
    while (!x.load(std::memory_order_seq_cst))
        ;
    if (y.load(std::memory_order_seq_cst)) ++z;
}
void read_y_then_x() {
    while (!y.load(std::memory_order_seq_cst))
        ;
    if (x.load(std::memory_order_seq_cst)) ++z;
}

void seq_cst_test() {
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(read_y_then_x);
    a.join();
    b.join();
    c.join();
    d.join();
    assert(z.load() != 0);
    std::cout << z.load() << std::endl;
}

void my_atomic_test() {
    // 1, 2; 2, 1; the behavior is undefined
    foo_order(get_num(), get_num());
}

#endif