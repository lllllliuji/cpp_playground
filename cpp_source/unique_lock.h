#ifndef Uniquelock_h_
#define Uniquelock_h_

#include <mutex>
#include <thread>

class some_object {};
class unique_lock_example {
   private:
    some_object some_detail;
    std::mutex m;

   public:
    unique_lock_example(some_object const& sd) : some_detail(sd) {}
    friend void swap(unique_lock_example& lhs, unique_lock_example& rhs) {
        if (&lhs == &rhs) return;
        std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock);
        std::unique_lock<std::mutex> lock_b(rhs.m, std::defer_lock);
        std::lock(lock_a, lock_b);
        swap(lhs.some_detail, rhs.some_detail);
    }
};
void swap(some_object&, some_object&);

// Transferring mutex ownership between scopes
void prepare_data();
void do_something();
std::unique_lock<std::mutex> get_lock() {
    extern std::mutex some_mutex;
    std::unique_lock<std::mutex> lk(some_mutex);
    prepare_data();
    return lk;
}
void process_data() {
    std::unique_lock<std::mutex> lk(get_lock());
    do_something();
}

// example of communication
bool flag;
std::mutex m;
void wait_for_flag() {
    std::unique_lock<std::mutex> lk(m);
    while (!flag) {
        lk.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lk.lock();
    }
}

// unique lock is of great flexibility
/*
void get_and_process_data() {
    std::unique_lock<std::mutex> my_lock(the_mutex);
    some_class data_to_process = get_next_data_chunk();
    my_lock.unlock();
    result_type result = process(data_to_process);
    my_lock.lock();
    write_result(data_to_process, result);
}
*/
#endif