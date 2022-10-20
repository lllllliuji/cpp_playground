#ifndef Page_h_
#define Page_h_

#include <iostream>

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

void PageTest() {
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
}

#endif