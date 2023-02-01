#include <iostream>
#include <fstream>

int main() {
    std::ifstream infile("input.txt");
    std::ofstream outfile("output.txt");
    std::string str;
    while (infile.good()) {
        std::getline(infile, str);
        str.erase(str.begin());
        str.erase(str.begin());
        outfile << str << std::endl;
    }
    return 0;
}