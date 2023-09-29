#ifndef Singleton_h_
#define Singleton_h_

#include <iostream>
#include <mutex>

class Singleton {
   public:
    // 获取单例实例的静态成员函数
    static Singleton& getInstance() {
        // 使用 call_once 保证 init 函数只被调用一次
        std::call_once(initFlag, &Singleton::init);
        return *instance;
    }

        // 示例的业务函数
    void doSomething() { std::cout << "Singleton is doing something." << std::endl; }

   private:
    // 私有构造函数，确保不能直接实例化
    Singleton() {}

    // 静态成员变量，指向唯一的实例
    static Singleton* instance;

    // 用于保证 init 函数只被调用一次的标志
    static std::once_flag initFlag;

    // 实际的初始化逻辑
    static void init() { instance = new Singleton(); }
};

// 静态成员变量初始化
Singleton* Singleton::instance = nullptr;
std::once_flag Singleton::initFlag;

int singleton_test() {
    // 获取单例实例
    Singleton& singleton = Singleton::getInstance();

    // 使用单例实例
    singleton.doSomething();

    return 0;
}
#endif