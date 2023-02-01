#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

#include "random.h"

void UpperBoundTest() {
    using E = std::pair<int, int>;
    E arr[10];
    for (int i = 0; i < 10; i++) {
        arr[i] = std::make_pair<int, int>(Rand(0, 100), Rand(0, 100));
    }
    for (int i = 0; i < 10; i++) {
        std::cout << "{" << arr[i].first << ", " << arr[i].second << "}"
                  << ", ";
    }
    std::cout << std::endl;
    std::sort(arr, arr + 10, [](const auto& pa, const auto& pb) {
        if (pa.first != pb.first)
            return pa.first < pb.first;
        else
            return pa.second < pb.second;
    });
    for (int i = 0; i < 10; i++) {
        std::cout << "{" << arr[i].first << ", " << arr[i].second << "}"
                  << ", ";
    }
    std::cout << std::endl;
    auto it = std::upper_bound(arr, arr + 10, -1, [](int v, const auto& pii) { return v < pii.first; });
    if (int offset = it - arr; offset >= 0 && offset < 10) {
        std::cout << "offset: " << offset << ", " << (*it).first << " " << (*it).second << std::endl;
    } else {
        std::cout << "offset: " << offset << ", doesn't exist!" << std::endl;
    }
}

void LowerBoundTest() {
    using E = std::pair<int, int>;
    std::vector<E> vec;

    for (int i = 0; i < 10; i++) {
        vec.emplace_back(Rand(0, 10), i);
    }
    for (int i = 0; i < 10; i++) {
        std::cout << "{" << vec[i].first << ", " << vec[i].second << "}"
                  << ", ";
    }
    std::cout << std::endl;
    std::sort(vec.begin(), vec.end(), [](const auto& pa, const auto& pb) {
        if (pa.first != pb.first)
            return pa.first < pb.first;
        else
            return pa.second < pb.second;
    });
    for (int i = 0; i < 10; i++) {
        std::cout << "{" << vec[i].first << ", " << vec[i].second << "}"
                  << ", ";
    }
    std::cout << std::endl;
    auto it = std::lower_bound(vec.begin(), vec.end(), 3, [](const auto& pii, int v) { return pii.first < v; });
    if (it != vec.end()) {
        std::cout << (*it).first << " " << (*it).second << std::endl;
    } else {
        std::cout << "doesn't exist!" << std::endl;
    }
}