#ifndef Copy_control_h_
#define Copy_control_h_

#include "base.h"
class A {
   private:
    std::unique_ptr<std::vector<int>> p;
    int len;

   public:
    A() : len(0) { p = std::make_unique<std::vector<int>>(); };
    A(int len_) : len(len_) {
        p = std::make_unique<std::vector<int>>();
        for (int i = 0; i < len; i++) {
            p->push_back(std::rand());
        }
    }
    A(const A& a) {
        if (this == &a) return;
        len = a.len;
        *p = *(a.p);
    }
    A(A&& a) {
        if (this == &a) return;
        len = a.len;
        p = std::move(a.p);
    }
    A& operator=(const A& a) noexcept {
        if (this == &a) return *this;
        len = a.len;
        *p = *(a.p);
        return *this;
    }
    A& operator=(A&& a) noexcept {
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
};

#endif