
#include <string>
#include <stack.h>
#include <iostream>

int main()
{
    vart::Stack<std::string> test;

    test.Push("test");

    vart::Stack<std::string> test2(test);
    vart::Stack<std::string> test3;
    test3 = test2;
    
    auto r = test.Top();
    test.Pop();
    std::cout << r << '\n';

    return 0;
}
