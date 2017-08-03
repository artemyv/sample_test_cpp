#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <deque>
#include <string>

template <typename Container, typename Index>
decltype(auto)
authAndAccess(Container&& c, Index i)
{
    return std::forward<Container> (c)[i];
}

std::deque<std::string> makeStringDeque()
{
    std::deque<std::string> a;
    return a;
}


template<class T, typename ...Args>
int wrap(T* obj, int (T::*f)(Args...), Args... args) 
{
    try{
        return (obj->*f)(args...);
    }
    catch(...){
        return -1;
    }
}

class Test
{
    private:
    int ix(){return 0;}
    int iy(int y){return y;}
    int iz(int y, int z){return y+z;}
    
    public:
    int x(){ return wrap<Test>(this,&Test::ix);}
    int y(int a){ return wrap<Test,int>(this,&Test::iy,a);}
    int z(int a, int b){ return wrap<Test,int,int>(this,&Test::iz,a,b);}
};
int main() {
    auto s = authAndAccess(makeStringDeque(),5);
       Test t;
       std::cout << t.x() << std::endl;
       std::cout << t.y(1) << std::endl;
       std::cout << t.z(1,2) << std::endl;
    return 0;
}
