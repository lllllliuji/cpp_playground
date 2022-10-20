#include <memory>
#include <iostream>
class A {
   public:
    int a_;
    A() {}
    A(int a) : a_(a) {}
    void print() { std::cout << a_ << " " << std::endl; }
};
int main() {
    std::shared_ptr<A> aPtr;
    {
        std::shared_ptr<A> ptr = std::make_shared<A>(15);
        aPtr = ptr;
        std::cout << ptr.use_count() << " " << aPtr.use_count() << std::endl;
    }
    std::weak_ptr<A> wPtr = aPtr;
    std::cout << aPtr.use_count() << std::endl;
    if (!wPtr.expired()) {
        auto w = wPtr.lock();
        w->print();
    }
    auto bPtr = std::make_shared<A>(25);
    wPtr = bPtr;
    if (!wPtr.expired()) {
        auto w = wPtr.lock();
        w->print();
    }
    std::shared_ptr<A> ptr = std::make_shared<A>(19);
    std::cout << ptr.use_count() << std::endl;
    auto aptr = std::move(ptr);
    std::cout << ptr.use_count() << std::endl;
    aptr->print();
    return 0;
}