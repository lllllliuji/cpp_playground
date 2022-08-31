#ifndef Friend_h_
#define Friend_h_
#include <iostream>

class Address;
class PrintAddr;
class Student {
public:
    void showAddr(Address*);
    Student(std::string, int, double);
public:
    friend void show(Student *stu);
private:
    std::string name;
    int age;
    double score;
};

class Address {
public: 
    Address(std::string, std::string, std::string);
public:
    friend class PrintAddr;
    friend void Student::showAddr(Address*);
private:
    std::string province;
    std::string city;
    std::string distrinct;
};

class PrintAddr {
public:
    void print(Address* addr);
};

//Student成员函数实现
Student::Student(std::string _name, int _age, double _score): name(_name), age(_age), score(_score) {};
//将student的成员函数声明为address的友元函数
void Student::showAddr(Address *addr) {
    std::cout << "address: " << addr->province << " " << addr->city << " " << addr->distrinct << std::endl;
}

//Adress成员函数实现
Address::Address(std::string _province, std::string _city, std::string _distrinct): province(_province), city(_city), distrinct(_distrinct) {}

//PrintAddress成员函数实现
void PrintAddr::print(Address *addr) {
    std::cout << "address: " << addr->province << " " << addr->city << " " << addr->distrinct << std::endl;
}
//全局友元函数
void show(Student *stu) {
    std::cout << "name: " << stu->name << " age: " << stu->age << " score: " << stu->score << std::endl;
}



void friend_function_test() {
    Student stu("小明", 15, 90.6);
    show(&stu);  //调用友元函数
    Student *pstu = new Student("李磊", 16, 80.5);
    show(pstu);  

    Address addr("陕西", "西安", "雁塔");
    stu.showAddr(&addr);

    Address paddr("河北", "衡水", "桃城");
    pstu->showAddr(&paddr);

    PrintAddr printaddr;
    printaddr.print(&addr);
    printaddr.print(&paddr);
}
#endif