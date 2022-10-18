#include "test.h"
class Page {
   private:
    bool is_leaf_;

   public:
    void SetLeaf(bool is_leaf) { is_leaf_ = is_leaf; }
    bool IsLeaf() { return is_leaf_; }
};
class InternalPage : public Page {
   private:
    int arr_[1];
};
class LeafPage : public Page {
   private:
    int next_page_id_;
    int arr_[1];

   public:
    int KeyAt(int index) { return *(arr_ + index); }
    void Print(int length) {
        for (int i = 0; i < length; i++) {
            std::cout << KeyAt(i) << " ";
        }
    }
    int GetNextPageId() { return next_page_id_; }
};

class C {
   public:
    template <typename T>
    void print(T arr[], int length) {
        for (int i = 0; i < length; i++) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }
};
int main() {
    LeafPage *page = new LeafPage();
    page->SetLeaf(true);
    InternalPage *l_page = reinterpret_cast<InternalPage *>(page);
    std::cout << l_page->IsLeaf() << std::endl;

    int *ptr = (int *)malloc(sizeof(int) * 4);
    for (int i = 0; i < 4; i++) {
        *(ptr + i) = 4 - i;
    }
    std::cout << "=======" << std::endl;
    auto page_ptr = reinterpret_cast<LeafPage *>(ptr);
    std::cout << page_ptr->IsLeaf() << std::endl;
    std::cout << page_ptr->GetNextPageId() << std::endl;
    page_ptr->Print(2);
    std::mutex mut;
    std::unique_lock<std::mutex> lk(mut);
    C c;
    int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    double b[10] = {0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9};
    c.print(a, 10);
    c.print(b, 10);
    return 0;
}