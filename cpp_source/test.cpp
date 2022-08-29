#include "test.h"

class Node {
   private:
    int val;
    Node *next;

   public:
    Node(): val(0), next(nullptr) {
        std::cout << "default constructor" << std::endl;
    }
    Node(const Node&) = default;
    Node(int v, Node *nxt) : val(v), next(nxt) {}
    Node(int v) : val(v), next(nullptr) {}
    Node(Node&& node) {
        std::cout << "move constructor" << std::endl;
        this->val = node.val;
        this->next = node.next;
        node.next = nullptr;
        node.val = 0;
    }
    ~Node() { std::cout << "deconstructor" << std::endl; }
    void setVal(int v) { this->val = v; }
    int getVal() { return this->val; }
};

int main() {
    Node a;
    a.setVal(99);
    std::unique_ptr<Node> p;
    if (!p) {
        std::cout << "p is nullptr\n";
    }
    std::unique_ptr<Node> np1 = std::make_unique<Node>(10, nullptr);
    {
        std::unique_ptr<Node> np = std::make_unique<Node>(a);
        np->setVal(111);
        std::cout << np->getVal() << std::endl;
    }
    std::cout << a.getVal() << " " << np1->getVal() << std::endl;
    std::cout << "here" << std::endl;
    return 0;
}