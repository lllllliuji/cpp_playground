#include "test.h"

int main() {
    int a = 0;
    std::cout << a << std::endl;
    threadsafe_lookup_table<std::string, int> t;
    t.add_or_update_mapping("hello", 101);
    std::cout << t.value_for("hello", 0) << std::endl;
    
    return 0;
}