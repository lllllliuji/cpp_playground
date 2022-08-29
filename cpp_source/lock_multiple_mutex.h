#ifndef Lock_multiple_mutex_h_
#define Lock_multiple_mutex_h_

#include <mutex>

class some_big_object {

};
void swap(some_big_object& lhs,some_big_object& rhs);
class X {
private:
    some_big_object some_detail;
    std::mutex m;
public:
    X(some_big_object const& sd): some_detail(sd) {}
    friend void swap(X& lhs, X& rhs) {
        if(&lhs == &rhs) return;
        std::lock(lhs.m, rhs.m);//all or nothing, if fail to aquire both, release the other
        std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
        std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
        swap(lhs.some_detail, rhs.some_detail);
    }
};


#endif