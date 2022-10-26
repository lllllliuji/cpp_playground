#ifndef Copy_control_h_
#define Copy_control_h_

#include "base.h"
class A {
   private:
    int len;
    std::unique_ptr<std::vector<int>> p;

   public:
    A() : len(0) { p = std::make_unique<std::vector<int>>(); };
    A(int len_) : len(len_) {
        p = std::make_unique<std::vector<int>>();
        for (int i = 0; i < len; i++) {
            p->push_back(std::rand());
        }
    }
    A(const A& a) {
        std::cout << "copy constructor" << std::endl;
        if (this == &a) return;
        len = a.len;
        p.reset(std::make_unique<std::vector<int>>(*a.p).release());
    }
    A(A&& a) {
        std::cout << "move constructor" << std::endl;
        if (this == &a) return;
        len = a.len;
        p = std::move(a.p);
    }
    A& operator=(const A& a) noexcept {
        std::cout << "assign" << std::endl;
        std::cout << "here" << std::endl;
        if (this == &a) return *this;
        len = a.len;
        p.reset(std::make_unique<std::vector<int>>(*a.p).release());
        return *this;
    }
    A& operator=(A&& a) noexcept {
        std::cout << "move assign" << std::endl;
        if (this == &a) return *this;
        len = a.len;
        p = std::move(a.p);
        return *this;
    }
    void print() {
        if (!p) {
            std::cout << "nullptr" << std::endl;
            return;
        }
        for (int i = 0; i < (*p).size(); i++) {
            std::cout << (*p)[i] << " ";
        }
        std::cout << std::endl;
    }
    void reverse() {
        if (!p) return;
        std::reverse((*p).begin(), (*p).end());
    }
    int& at(int index) { return p->at(index); }
    int& front() { return p->front(); }
    int& back() { return p->back(); }
};

void copy_control_test() {
    A a;
    auto b = std::move(a);
    A c;
    c = std::move(b);
} 
#endif