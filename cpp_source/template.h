#ifndef Template_h_
#define Template_h_

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

void TemplateTest() {
    C c;
    int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    double b[10] = {0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9};
    c.print(a, 10);
    c.print(b, 10);
}
#endif