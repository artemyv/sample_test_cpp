// The C++ Programming Language
// Fourth Edition
// Bjarne Stroustrup
// 33.4 Function Objects

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

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
    sort(v.begin(),v.end(),greater<int>{});
    print(v);
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
5
4
3
2
1
*************************************/