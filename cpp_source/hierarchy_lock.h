#ifndef Hierarchy_lock_h_
#define Hierarchy_lock_h_

#include <exception>
#include <iostream>
#include <mutex>
#include <thread>

class hierarchical_mutex {
   private:
    std::mutex internal_mutex;
    unsigned long const hierarchy_value;
    unsigned long previous_hierarchy_value;
    static thread_local unsigned long this_thread_hierarchy_value;
    void check_for_hierarchy_violation() {
        if (this_thread_hierarchy_value <= hierarchy_value) {
            throw std::logic_error("mutex hierarchy violated");
        }
    }
    void update_hierarchy_value() {
        previous_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;
    }

   public:
    explicit hierarchical_mutex(unsigned long value)
        : hierarchy_value(value), previous_hierarchy_value(0) {}
    void lock() {
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }
    void unlock() {
        if (this_thread_hierarchy_value != hierarchy_value)
            throw std::logic_error("mutex hierarchy violated");
        this_thread_hierarchy_value = previous_hierarchy_value;
        internal_mutex.unlock();
    }
    bool try_lock() {
        check_for_hierarchy_violation();
        if (!internal_mutex.try_lock()) return false;
        update_hierarchy_value();
        return true;
    }
};
thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(
    ULONG_MAX);

// example
hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex low_level_mutex(5000);
hierarchical_mutex other_mutex(6000);
int do_low_level_stuff() {
    std::cout << "low level" << std::endl;
    return 0;
}
int low_level_func() {
    std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
    return do_low_level_stuff();
}
void high_level_stuff(int some_param) {
    std::cout << "high level" << std::endl;
}
void high_level_func() {
    std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
    high_level_stuff(low_level_func());
}
void thread_a() { high_level_func(); }
void do_other_stuff() { std::cout << "other stuff" << std::endl; }
void other_stuff() {
    high_level_func();
    do_other_stuff();
}
void thread_b() {
    std::lock_guard<hierarchical_mutex> lk(other_mutex);
    other_stuff();  // would fail, because now level is 6000, try to lock 10000
}

void hierarchy_lock_test() {
    std::thread a(thread_a);
    a.join();
    std::cout << "high level call low level success" << std::endl;
    std::thread b(thread_b);
    b.join();
    std::cout << "low level call high level success" << std::endl;
}
#endif