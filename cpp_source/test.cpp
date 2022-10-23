#include "test.h"

int main() {
    uint32_t a = UINT32_MAX;
    a = a + 10;
    std::cout << a << std::endl;
    std::cout << (1ULL << 32) - 1 << std::endl;
    uint32_t b = 1, c = 0;
    std::cout << c - b << std::endl;
    return 0;
}