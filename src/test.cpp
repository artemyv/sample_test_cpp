//Testing Item 8: Prefer nullptr to 0 and NULL .
//From Effective Modern C++
//  42 SPECIFIC WAYS TO IMPROVE YOUR USE OF C++11 AND C++14
//  by Scott Meyers

#include <mutex>
#include <memory>
#include <iostream>
class Widget
{

};

int f1(std::shared_ptr<Widget> )    // call these only when
{
    return 0;
}
double f2(std::unique_ptr<Widget> ) // the appropriate
{
    return 1.0;
}
bool f3(Widget*)                   // mutex is locked
{
    return true;
}

std::mutex f1m, f2m, f3m; // mutexes for f1, f2, and f3

using MuxGuard = std::lock_guard<std::mutex>;

template<typename FuncType, typename MuxType, typename PtrType>
    decltype(auto) lockAndCall(FuncType func, MuxType& mutex, PtrType ptr)
{
    MuxGuard g(mutex);
    return func(ptr);
}

int main() {
    auto result1 = lockAndCall(f1, f1m, 0); // error!
    auto result2 = lockAndCall(f2, f2m, NULL);// error!
    auto result3 = lockAndCall(f3, f3m, nullptr);
    std::cout << result1 << ' ' << result2 << ' '    << result3     << std::endl;
    return 0;
}
