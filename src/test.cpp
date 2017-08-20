// The C++ Programming Language
// Fourth Edition
// Bjarne Stroustrup
// 33.3 Range Access Functions

#include <iostream>
#include <iterator>
#include <vector>

using namespace std;
template<typename Cont>
void print(Cont& c)
{
    for(auto p=begin(c); p!=end(c); ++p)
        cout << *p << '\n';
}
void f()
{
    vector<int> v {1,2,3,4,5};
    print(v);
    int a[] {1,2,3,4,5};
    print(a);
}

int main()
{
    f();
    return 0;

}

/*************************************
Output
$ ./test
1
2
3
4
5
1
2
3
4
5
*************************************/