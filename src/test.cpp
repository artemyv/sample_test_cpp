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

int main() {
   HomeForSale h;
   std::vector<HomeForSale> v(1);
   v.push_back(std::move(h));
    return 0;
}
