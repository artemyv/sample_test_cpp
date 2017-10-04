https://stackoverflow.com/questions/46556985/how-do-i-format-my-function-to-call-a-templated-class#46556985

#include <iostream>

template <typename T>
class Foo {
    T t_;
public:
    Foo(const T& t):t_(t){};
    void print() {std::cout << t_ << '\n'; }
};

class noFoo {
public:
    template <typename T>
    void test(Foo <T>& foo);
};

template <typename T>
void noFoo::test(Foo <T>& foo)
{
    foo.print();
}
int main() {
    Foo<char> wr('A');
    Foo<int> wi(5);
    noFoo n;
    n.test(wr);
    n.test(wi);
    return 0;
}