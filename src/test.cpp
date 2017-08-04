#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <deque>
#include <string>
#include <vector>
class Uncopyable {
protected:
Uncopyable() {}
~Uncopyable() {}
private:
Uncopyable(const Uncopyable&);
Uncopyable& operator=(const Uncopyable&);
};

class HomeForSale: private Uncopyable {
public:
 int unused;
    HomeForSale():unused(0){}
    HomeForSale(HomeForSale&& h)
 {
     unused = h.unused;
 }
};
template<typename T>
class TD;

auto derefLess =
    [](const auto& p1, const auto& p2)
        { return *p1 < *p2; };

bool operator< (const HomeForSale& lhs, const HomeForSale& rhs){
    return lhs.unused < rhs.unused;
}
int main() {
   HomeForSale h;
   HomeForSale u;
   if(h<u){}
   if(derefLess(&h,&u)){}
    return 0;
}
