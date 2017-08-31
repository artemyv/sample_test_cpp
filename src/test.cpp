
//Exceptional C++ Item19 sample

#include <string>
#include <memory>
#include <iostream>

std::unique_ptr<std::string> GetEmpl(std::string title) //copy constructor can throw
{
    //constructor from const char*
    //operator +
    //copy constructor
    std::unique_ptr<std::string> res = std::make_unique<std::string>(title + " name");


    //constructor from const char*
    //operator ==
    if(title == "CEO") {
        //constructor from const char*
        //operator +
        std::string msg = *res + " overpaid\n";
        //operator <<
        std::cout <<  msg;
    }

    //move semantics - cannot throw
    return res;
}

int main()
{
    //can throw before and during call
    //not after the call when assigning result to res
    auto res = GetEmpl("CEO");

    //operator <<
    std::cout << *res << '\n';

    return 0;
}
