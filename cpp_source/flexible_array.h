#pragma once
#include <iostream>

struct SomeClass {
    long data_length;
    long data[1];
    // long data[0] is ok too
};

void flexible_array_test() {
    long *p = (long *)malloc(10 * sizeof(long));
    for (int i = 0; i < 10; i++) {
        std::cout << p[i] << " ";
    }
    std::cout << std::endl;
    SomeClass *sp = (SomeClass *)p;
    std::cout << sp->data_length << " ";
    for (int i = 0; i < 9; i++) {
        std::cout << sp->data[i] << " ";
    }
}
