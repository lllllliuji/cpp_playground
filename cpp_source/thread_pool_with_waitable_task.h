#ifndef Thread_pool_with_waitable_task_h_
#define Thread_pool_with_waitable_task_h_

#include "base.h"
#include "join_threads.h"
#include "thread_safe_queue.h"
class function_wrapper {
    struct impl_base {
        virtual void call() = 0;
        virtual ~impl_base() {}
    };
    std::unique_ptr<impl_base> impl;
    template <typename F>
    struct impl_type : impl_base {
        F f;
        impl_type(F&& f_) : f(std::move(f_)) {}
        void call() { f(); }
    };

   public:
    template <typename F>
    function_wrapper(F&& f) : impl(new impl_type<F>(std::move(f))) {}
    void operator()() { impl->call(); }
    function_wrapper() = default;
    function_wrapper(function_wrapper&& other) : impl(std::move(other.impl)) {}
    function_wrapper& operator=(function_wrapper&& other) {
        impl = std::move(other.impl);
        return *this;
    }
    function_wrapper(const function_wrapper&) = delete;
    function_wrapper(function_wrapper&) = delete;
    function_wrapper& operator=(const function_wrapper&) = delete;
};
class thread_pool {
    threadsafe_queue<function_wrapper> work_queue;
    std::atomic_bool done;
    std::vector<std::thread> threads;
    join_threads joiner;

    void worker_thread() {
        while (!done) {
            function_wrapper task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }

   public:
    thread_pool() : done(false), joiner(threads) {
        unsigned const thread_count = std::thread::hardware_concurrency();
        try {
            for (unsigned i = 0; i < thread_count; ++i) {
                threads.push_back(std::thread(&thread_pool::worker_thread, this));
            }
        } catch (...) {
            done = true;
            throw;
        }
    }
    ~thread_pool() { done = true; }

   public:
    template <typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f) {
        typedef typename std::result_of<FunctionType()>::type result_type;
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());
        work_queue.push(std::move(task));
        return res;
    }
};
template <typename Iterator, typename T>
struct accumulate_block_2 {
    T operator()(Iterator first, Iterator last, T init) { 
        return std::accumulate(first, last, init); 
    }
};
template <typename Iterator, typename T>
T parallel_accumulate_with_thread_pool(Iterator first, Iterator last, T init) {
    unsigned long const length = std::distance(first, last);
    if (!length) return init;
    unsigned long const block_size = 25;
    unsigned long const num_blocks = (length + block_size - 1) / block_size;
    std::vector<std::future<T> > futures(num_blocks - 1);
    thread_pool pool;
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_blocks - 1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        futures[i] = pool.submit([=] { accumulate_block_2<Iterator, T>()(block_start, block_end, init); });
        block_start = block_end;
    }
    T last_result = accumulate_block_2<Iterator, T>()(block_start, last, init);
    T result = init;
    for (unsigned long i = 0; i < (num_blocks - 1); ++i) {
        result += futures[i].get();
    }
    result += last_result;
    return result;
}


void thread_pool_with_waitable_task_test() {
    std::vector<int> nums;
    for (int i = 0; i < 10000100; i++) {
        nums.emplace_back(std::rand());
    }
    std::cout << std::accumulate(nums.begin(), nums.end(), 0LL) << std::endl;
    std::cout << parallel_accumulate_with_thread_pool(nums.begin(), nums.end(), 0LL) << std::endl;
}
#endif