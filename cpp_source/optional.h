#ifndef Optional_h_
#define Optional_h_

#include <functional>
#include <iostream>
#include <optional>
#include <string>

// optional can be used as the return type of a factory that may fail
std::optional<std::string> create(bool b) {
    if (b) return "Godzilla";
    return {};
}

// std::nullopt can be used to create any (empty) std::optional
auto create2(bool b) { return b ? std::optional<std::string>{"Godzilla"} : std::nullopt; }

// std::reference_wrapper may be used to return a reference
auto create_ref(bool b) {
    static std::string value = "Godzilla";
    return b ? std::optional<std::reference_wrapper<std::string>>{value} : std::nullopt;
}

void optional_test() {
    std::optional<std::string> op_str = std::make_optional<std::string>("hello");
    op_str.reset();
    std::cout << "op_str: " << op_str.value_or("world") << std::endl;
    std::cout << "create(false) returned " << create(false).value_or("empty") << '\n';

    // optional-returning factory functions are usable as conditions of while and if
    if (auto str = create2(true)) {
        std::cout << "create2(true) returned " << *str << '\n';
    }

    if (auto str = create_ref(true)) {
        // using get() to access the reference_wrapper's value
        std::cout << "create_ref(true) returned " << str->get() << '\n';
        str->get() = "Mothra";
        std::cout << "modifying it changed it to " << str->get() << '\n';
    }
}

#endif