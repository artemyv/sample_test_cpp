//https://stackoverflow.com/questions/45877110/polymorphism-with-shared-ptr-of-base-class-with-3-level-inheritance
#include <iostream>
#include <memory>

class Base {
public:
    virtual void func() = 0;
};


class Derived : public Base {
public:
    void func() {std::cout << "Derived\n"; }
};

class DerivedDerived : public Derived {
public:
    void func() {std::cout << "DerivedDerived\n";} //Overrides func() from Derived
};
void g(std::shared_ptr<Base> s) {
    s->func();
}

int main()
{
    std::shared_ptr<Base> s1 = std::make_shared<Derived>();
    std::shared_ptr<Base> s2 = std::make_shared<DerivedDerived>();
    g(s1); //func() of Derived Class called
    g(s2); //func() of Derived Class called, func() of DerivedDerived Class needed to
    return 0;
}

/***************
Output:
$ ./test
Derived
DerivedDerived
***************/