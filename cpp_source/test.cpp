#include "test.h"

int main() {
    std::vector<std::pair<int, double>> vec;
    for (int i = 0; i < 10; i++) {
        vec.push_back(std::make_pair(i, 1.0 * i));
    }
    long long ans = 0;
    std::for_each(vec.begin(), vec.end(), [&](const auto& item) { ans += item.first; });
    std::cout << ans << std::endl;
    return 0;
}