#include "test.h"

int main() {
    auto min = gradient_descent(0.01, 0.01, [](double a) { return cos(a); });
    std::cout << "Result: " << min << std::endl;

    return 0;
}
