#include "test.h"
int main() {
    std::ifstream infile("./clock_test.cpp");
    std::string str;
    while (infile.good()) {
        std::getline(infile, str);
        std::cout << str << std::endl;
    }
    return 0;
}