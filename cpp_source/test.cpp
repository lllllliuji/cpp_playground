#include "test.h"

int main() {
    std::list<std::string> strs;

    for (int i = 0; i < 20; i++) {
        strs.emplace_back(std::to_string(std::rand() % 100000));
    }
    auto result = parallel_quick_sort(strs);
    for (const auto& item : result) {
        std::cout << item << " ";
    }

    return 0;
}

/*
    std::_List_const_iterator<std::pair<std::__cxx11::basic_string<char>, int> >
   const std::__cxx11::list<std::pair<std::__cxx11::basic_string<char>, int>,
   std::allocator<std::pair<std::__cxx11::basic_string<char>, int> > >::iterator

*/