//https://stackoverflow.com/questions/46188666/why-does-template-argument-deduction-substitution-fail-with-stdtuple-and-templ
// Example program
#include <iostream>
#include <tuple>

template<typename T>
struct Base {};

template<typename T>
struct Derived1 : Base<T> {};

template<typename T>
struct Derived2 : Base<T> {};

template<typename T1, typename T2>
void function(std::tuple<Base<T1>&,Base<T2>&> arg)
{
    std::cout << "Hello\n";
}

int main()
{
    Derived1<int> d1;
    Derived2<double> d2;

    function(std::tie(d1, d2));  /* In this case the template argument deduction/substitution failed */
    function<int,double>(std::tie(d1, d2)); /* here works */

    Base<int>& b1 = d1;
    Base<double>& b2 = d2;

    function(std::tie(b1, b2)); /* but, in this case also works */
}