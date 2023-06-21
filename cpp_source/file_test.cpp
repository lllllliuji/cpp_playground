#include <fstream>
#include <iostream>
#include <string>
int main() {
    std::ifstream infile("/autograder/source/bustub/test/storage/grading_b_plus_tree_checkpoint_2_concurrent_test.cpp");
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