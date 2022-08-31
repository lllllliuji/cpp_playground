#ifndef Thread_safe_queue_h_
#define Thread_safe_queue_h_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
template <typename T>
class threadsafe_queue {
   private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

   public:
    threadsafe_queue() {}
    threadsafe_queue(threadsafe_queue const &other) {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    }
    void push(T &&new_value) {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(std::move(new_value));
        data_cond.notify_one();
    }
    void wait_and_pop(T &value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        value = std::move(data_queue.front());
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }
    bool try_pop(T &value) {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty()) return false;
        value = std::move(data_queue.front());
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty()) return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }
    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};

/*
//another implement with less critical zone
template <typename T>
class threadsafe_queue {
   private:
    mutable std::mutex mut;
    std::queue<std::shared_ptr<T> > data_queue;
    std::condition_variable data_cond;

   public:
    threadsafe_queue() {}
    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        value = std::move(*data_queue.front());
        data_queue.pop();
    }
    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty()) return false;
        value = std::move(*data_queue.front());
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        std::shared_ptr<T> res = data_queue.front();

        data_queue.pop();
        return res;
    }
    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty()) return std::shared_ptr<T>();
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }
    void push(T new_value) {
        std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }
    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
    size_t size() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.size();
    }
};
*/

void test_concurrent_push_and_pop_on_empty_queue() {
    threadsafe_queue<int> q;
    std::promise<void> go, push_ready, pop_ready;
    std::shared_future<void> ready(go.get_future());
    std::future<void> push_done;
    std::future<int> pop_done;
    try {
        push_done = std::async(std::launch::async, [&q, ready, &push_ready]() {
            push_ready.set_value();
            ready.wait();
            q.push(42);
        });
        pop_done = std::async(std::launch::async, [&q, ready, &pop_ready]() {
            pop_ready.set_value();
            ready.wait();
            return *q.try_pop();
        });
        push_ready.get_future().wait();
        pop_ready.get_future().wait();
        go.set_value();
        push_done.get();
        assert(pop_done.get() == 42);
        assert(q.empty());
    } catch (...) {
        go.set_value();
        throw;
    }
}
#endif