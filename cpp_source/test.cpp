#include "test.h"
int main() {
    std::ifstream infile("tmp.cpp");
    std::ofstream outfile("buffer_pool_manager.cpp");
    std::string str;
    while (infile.good()) {
        std::getline(infile, str);
        str.erase(str.begin());
        str.erase(str.begin());
        outfile << str << std::endl;
        std::cout << str << std::endl;
    }
    return 0;
}