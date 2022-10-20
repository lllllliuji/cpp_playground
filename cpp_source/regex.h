#ifndef Regex_h_
#define Regex_h_

#include <iostream>
#include <regex>
#include <vector>
#include <string>

void Stringsplit(const std::string& str, const std::string& split, std::vector<std::string>& res) {
    // std::regex ws_re("\\s+"); // 正则表达式,匹配空格
    std::regex reg(split);  // 匹配split
    std::sregex_token_iterator pos(str.begin(), str.end(), reg, -1);
    decltype(pos) end;  // 自动推导类型
    for (; pos != end; ++pos) {
        res.push_back(pos->str());
    }
}

void RegexTest() {
    std::string test_string;
    for (int i = 0; i < 10; i++) {
        test_string += std::to_string(std::rand()) + "\r\n";
    }
    std::vector<std::string> strs;
    Stringsplit(test_string, "\r\n", strs);
    for (auto& str : strs) {
        std::cout << str << std::endl;
    }
}
#endif