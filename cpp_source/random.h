#ifndef Random_h_
#define Random_h_

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

void RandTest() {
    // 随机非负数 伪随机
    std::cout << "default_ random_engine" << std::endl;
    std::random_device rd;
    std::default_random_engine e(rd());
    std::mt19937 eng(rd());

    for (int i = 0; i < 10; i++) {
        std::cout << e() << " ";
    }
    std::cout << std::endl;

    std::uniform_int_distribution<int> u(-1, 1);
    for (int i = 0; i < 10; i++) {
        std::cout << u(e) << " ";
    }
    std::cout << std::endl;

    std::uniform_real_distribution<double> ud(0.0, 1.1);
    for (int i = 0; i < 10; i++) {
        std::cout << ud(e) << " ";
    }
    std::cout << std::endl;

    std::bernoulli_distribution ub;
    for (int i = 0; i < 10; i++) {
        std::cout << ub(e) << " ";
    }
    std::cout << std::endl;

    std::cout << "mt19937" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << eng() << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < 10; i++) {
        std::cout << u(eng) << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < 10; i++) {
        std::cout << ud(eng) << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < 10; i++) {
        std::cout << u(eng) << " ";
    }
    std::cout << std::endl;
}
int Rand(int low, int high) {
    std::random_device rd;
    std::default_random_engine e(rd());
    std::uniform_int_distribution<int> u(low, high);
    return u(e);
}

std::string Rand(int low, int high, int len) {
    std::stringstream ss;
    std::string rand_suffix;
    for (int i = 0; i < 10; i++) {
        ss << std::setw(4) << std::setfill('0') << Rand(0, 1000) << " ";
        ss >> rand_suffix;
        std::cout << rand_suffix << std::endl;
    }
    return rand_suffix;
}
#endif