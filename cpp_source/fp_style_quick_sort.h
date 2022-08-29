#ifndef Fp_style_quick_sort
#define Fp_style_quick_sort

#include <list>
#include <algorithm>

template <typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
    if (input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    T const &pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(), [&](T const &t) { return t < pivot; });
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
    auto new_lower(sequential_quick_sort(std::move(lower_part)));
    auto new_higher(sequential_quick_sort(std::move(input)));
    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower);
    return result;
}

template <typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
    if (input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    T const &pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(), [&](T const &t) { return t < pivot; });
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
    std::future<std::list<T>> new_lower(std::async(&parallel_quick_sort<T>, std::move(lower_part)));
    auto new_higher(parallel_quick_sort(std::move(input)));
    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower.get());
    return result;
}


void fp_quick_sort_test() {
    // initializing lists
    std::list<int> l1 = {1, 2, 3};
    std::list<int> l2 = {4, 5};
    std::list<int> l3 = {6, 7, 8};

    // transfer all the elements of l2
    l1.splice(l1.begin(), l2);

    // at the beginning of l1
    std::cout << "list l1 after splice operation" << std::endl;
    for (auto x : l1)
        std::cout << x << " ";

    // transfer all the elements of l1
    l3.splice(l3.end(), l1);

    // at the end of l3
    std::cout << "\nlist l3 after splice operation" << std::endl;
    for (auto x : l3)
        std::cout << x << " ";
    auto result = parallel_quick_sort(l3);
    std::cout << std::endl;
    for (auto &x : result)
    {
        std::cout << x << " ";
    }
}
#endif