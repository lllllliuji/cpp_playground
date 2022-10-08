#include <iostream>
#include <fstream>

int main() {
    std::ifstream file("./hello.cpp");
    std::string str;
    while (file.good()) {
        std::getline(file, str);
        std::cout << str << std::endl;
    }
    return 0;
}