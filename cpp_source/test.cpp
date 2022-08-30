#include "test.h"

int main() {
    int a = 0;
    std::cout << a << std::endl;
    threadsafe_lookup_table<std::string, int> t;
    std::string h("hello");
    t.add_or_update_mapping(h, 101);
    std::cout << t.value_for(h, 0) << std::endl;
    
    return 0;
}

/*
    std::_List_const_iterator<std::pair<std::__cxx11::basic_string<char>, int> >
   const std::__cxx11::list<std::pair<std::__cxx11::basic_string<char>, int>, std::allocator<std::pair<std::__cxx11::basic_string<char>, int> > >::iterator

*/