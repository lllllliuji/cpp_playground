#include "base.h"
void print_pack(std::tuple<std::string&&, int&&> pack) {
    std::cout << std::get<0>(pack) << ", " << std::get<1>(pack) << '\n';
}
int main() {
    std::tuple<int, char, double> tuple_a(10, 'a', 3.14);
    std::cout << "tuple_a has ";
    std::cout << std::tuple_size<decltype(tuple_a)>::value;
    std::cout << " elements." << '\n';

    auto tuple_b = std::make_tuple(10, 'a');
    std::tuple_element<0, decltype(tuple_b)>::type first = std::get<0>(tuple_b);
    std::tuple_element<1, decltype(tuple_b)>::type second = std::get<1>(tuple_b);
    std::cout << "tuple_b contains: " << first << " and " << second << '\n';

    std::string str("John");
    print_pack(std::forward_as_tuple(str + " Smith", 25));
    print_pack(std::forward_as_tuple(str + " Daniels", 22));

    int myint;
    char mychar;
    std::tuple<int, float, char> tuple_c;
    tuple_c = std::make_tuple(10, 2.6, 'a');         // packing values into tuple
    std::tie(myint, std::ignore, mychar) = tuple_c;  // unpacking tuple into variables
    std::cout << "myint contains: " << myint << '\n';
    std::cout << "mychar contains: " << mychar << '\n';

    std::tuple<float, std::string> tuple_d(3.14, "pi");
    std::pair<int, char> mypair(10, 'a');
    auto myauto = std::tuple_cat(tuple_c, std::tuple<int, char>(mypair));
    std::cout << "myauto contains: " << '\n';
    std::cout << std::get<0>(myauto) << '\n';
    std::cout << std::get<1>(myauto) << '\n';
    std::cout << std::get<2>(myauto) << '\n';
    std::cout << std::get<3>(myauto) << '\n';

    std::tuple<int, char> tuple_e(10, 'a');
    std::get<0>(tuple_e) = 20;
    std::cout << "tuple_e contains: ";
    std::cout << std::get<0>(tuple_e) << " and " << std::get<1>(tuple_e);
    std::cout << std::endl;
    return 0;
}