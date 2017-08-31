
//Exceptional C++ Item19 sample

#include <string>
#include <memory>
#include <iostream>

std::unique_ptr<std::string> GetEmpl(std::string title)
{
    std::unique_ptr<std::string> res = std::make_unique<std::string>(title + " name");
    if(title == "CEO"){
        std::string msg = *res + " overpaid\n";
        std::cout <<  msg;
    }
    return res;
}

int main()
{
    auto res = GetEmpl("CEO");
    std::cout << *res << '\n';

    return 0;
}
