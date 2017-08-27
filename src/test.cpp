// https://stackoverflow.com/questions/45902704/c11-copying-just-one-field-into-a-vector

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>


struct Foo {
    double a;
    int b;
 };

 void f(const std::initializer_list<Foo> &args)
 {
    std::vector<int> result;
    std::vector<int> result2;
    
    result.reserve(sizeof(args));
    result2.reserve(sizeof(args));
    std::transform(std::begin(args), std::end(args), std::back_inserter(result), [] (const Foo & foo) { return foo.b; });
    
    std::transform(std::begin(args), std::end(args), std::back_inserter(result2), std::mem_fn(&Foo::b));
    
    std::cout << "res1:";
    for(auto i:result){
        std::cout << ' ' << i;
    }
    std::cout << "\nres2:";
    for(auto i:result2){
        std::cout << ' ' << i;
    }
    std::cout << "\n";
}

int main()
{
    Foo a1 {0.5,1};
    Foo a2 {0.5,2};
    Foo a3 {0.5,3};
    f({a1,a2,a3});
    return 0;
}