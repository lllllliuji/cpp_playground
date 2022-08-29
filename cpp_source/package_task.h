#ifndef Package_task_h_
#define Package_task_h_

#include <mutex>
#include <iostream>
#include <future>
#include <functional>
#include <thread>
#include <deque>
#include <algorithm>
#include <numeric>
#include <vector>
#include <chrono>
#include <set>

std::vector<int> nums;

template <typename Iterator, typename T>
T my_accumulate(Iterator first, Iterator last, T init_value)
{
    return std::accumulate(first, last, init_value);
}

template <typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T &result)
    {
        result = std::accumulate(first, last, result);
    }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init_value)
{
    unsigned long const length = std::distance(first, last);
    if (!length)
        return init_value;

    unsigned long const min_per_thread = 10000;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;

    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);

    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }
    accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);
    for (auto &entry : threads)
    {
        entry.join();
    }
    return std::accumulate(results.begin(), results.end(), init_value);
}

// todo, write packaged_task with arguments

// void cal_thread() {
//     while (flag) {
//         std::packaged_task<long(E, E)> task;
//         {
//             std::lock_guard<std::mutex> lk(m);
//             if (tasks.empty()) {
//                 continue;
//             }
//             task = std::move(tasks.front());
//             tasks.pop_front();
//         }
//         task();
//     }
// }

// void add_thread() {
//     long n = nums.size();
//     std::set<int> fragment;
//     for (int i = 0; i < 100; i++) {
//         fragment.insert(std::rand() % n);
//     }
//     fragment.erase(0);
//     fragment.insert(n);
//     std::vector<int>::const_iterator start = nums.begin();
//     std::vector<int>::const_iterator block_start = start;

// }

void test_package_task()
{
    for (int i = 0; i < 100000000; i++)
    {
        nums.emplace_back(std::rand());
    }

    std::cout << "normal calculate" << std::endl;
    auto start = std::chrono::system_clock::now();
    auto result_from_normal_calculate = std::accumulate(nums.begin(), nums.end(), 0LL);
    auto end = std::chrono::system_clock::now();
    std::cout << "result = " << result_from_normal_calculate << std::endl;
    std::chrono::duration<double> cost = end - start;
    std::cout << "time cost: " << cost.count() << std::endl;

    std::cout << "use join to calculate" << std::endl;
    start = std::chrono::system_clock::now();
    auto result_from_parallel = parallel_accumulate(nums.begin(), nums.end(), 0LL);
    end = std::chrono::system_clock::now();
    std::cout << "result = " << result_from_parallel << std::endl;
    cost = end - start;
    std::cout << "time cost: " << cost.count() << std::endl;
}

#endif