#ifndef Simple_thread_pool_h_
#define Simple_thread_pool_h_

#include "base.h"
#include "join_threads.h"
#include "thread_safe_queue.h"

class simple_thread_pool {
    std::atomic_bool done;
    threadsafe_queue<std::function<void()> > work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;
    void worker_thread() {
        while (!done) {
            std::function<void()> task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }

   public:
    simple_thread_pool() : done(false), joiner(threads) {
        unsigned const thread_count = std::thread::hardware_concurrency();
        try {
            for (unsigned i = 0; i < thread_count; ++i) {
                threads.push_back(std::thread(&simple_thread_pool::worker_thread, this));
            }
        } catch (...) {
            done = true;
            throw;
        }
    }
    ~simple_thread_pool() { done = true; }
    template <typename FunctionType>
    void submit(FunctionType f) {
        work_queue.push(std::function<void()>(f));
    }
};

#endif