// Item 26: Avoid overloading on universal references.
// Item 27: Familiarize yourself with alternatives to
//          overloading on universal references.
//  42 SPECIFIC WAYS TO IMPROVE YOUR USE OF C++11 AND C++14
//  by Scott Meyers

//#include "widget.h"

#include <chrono>
#include <string>
#include <set>
void log (std::chrono::time_point<std::chrono::system_clock> , std::string ) {

}

std::string nameFromIdx(int idx)
{
    if(idx == 0) return "initial";
    return "final";
}


template<typename T>
void logAndAddImpl(T&& name, std::false_type)
{
    static std::multiset<std::string> names;
    auto now = std::chrono::system_clock::now();
    log(now, "logAndAdd");
    names.emplace(std::forward<T>(name));
}
void logAndAddImpl(int idx, std::true_type);

template<typename T>
void logAndAdd(T&& name)
{
    logAndAddImpl(
        std::forward<T>(name),
        std::is_integral<std::remove_reference_t<T>>()
    );
}
void logAndAddImpl(int idx, std::true_type)
{
    logAndAdd(nameFromIdx(idx));
}

int main() {
    std::string petName("Darla");
    logAndAdd(petName);     // pass lvalue std::string
    logAndAdd(std::string("Persephone")); // pass rvalue std::string
    logAndAdd("Patty Dog");
    return 0;
}
