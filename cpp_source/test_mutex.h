#ifndef Test_mutex_h
#define Test_mutex_h


#include <list>
#include <mutex>
#include <algorithm>


std::list<int> some_list;
std::mutex some_mutex;
void add_to_list(int new_value) {
    //RAII(Resouce Aquire is initialization) mutex.lock() in constructor
    std::lock_guard<std::mutex> guard(some_mutex);
    some_list.push_back(new_value);
    //lock_guard's destruction call here, then call mutex.unlock()
}
bool list_contains(int value_to_find) {
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}


#endif