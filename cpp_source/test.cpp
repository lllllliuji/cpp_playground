#include "test.h"

int main() {
    int a = 0;
    std::cout << a << std::endl;
    std::string h("hello");
    std::cout << std::hash<std::string>()(h) << std::endl;

    std::list<std::pair<std::string, int>> bucket;
    for (int i = 0; i < 100; i++) {
        auto p = std::make_pair(std::to_string(i), i);
        bucket.push_back(std::move(p));
    }
    auto it = std::find_if(bucket.begin(), bucket.end(), [](const auto &key) {
        return key.first == std::to_string(55);
    });
    it->second = 66;
    for (const auto& item : bucket) {
        if (item.first == std::to_string(55)) 
        std::cout << item.first << " " << item.second << std::endl;
    }
    return 0;
}

/*
    std::_List_const_iterator<std::pair<std::__cxx11::basic_string<char>, int> >
   const std::__cxx11::list<std::pair<std::__cxx11::basic_string<char>, int>, std::allocator<std::pair<std::__cxx11::basic_string<char>, int> > >::iterator

*/