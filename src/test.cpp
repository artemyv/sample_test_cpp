#include <vector>
#include <iostream>

using std::cout;

int cnt = 0;
class foo{
public:
    //constructor
    foo() : ucnt(++cnt) { cout << ucnt << "\tdefault\n" ; }
    foo(const foo&) : ucnt(++cnt) { cout << ucnt << "\tcopy\n" ; }
    foo(foo&&) noexcept : ucnt(++cnt)  { cout << ucnt << "\tmove\n" ; }
    ~foo() { cout << ucnt << "\tdestroyed\n" ; }
    int ucnt;
};

int main()
{
    std::vector<foo> vf = {foo()};
    cout << vf.size() << " 1: " << vf[0].ucnt << '\n';
    vf.reserve(3);
    cout << vf.size() << " 2: " << vf[0].ucnt << '\n';
    vf.push_back(foo());
    cout << vf.size() << " 3: " << vf[0].ucnt << " " << vf[1].ucnt << '\n';
    vf.emplace_back();
    cout << vf.size() << " 4: " << vf[0].ucnt << " " << vf[1].ucnt << " " << vf[2].ucnt << '\n';
    return 0;
}

/***************
Output
$ ./test
1       default
2       copy
1       destroyed
1 1: 2
3       move
2       destroyed
1 2: 3
4       default
5       move
4       destroyed
2 3: 3 5
6       default
3 4: 3 5 6
3       destroyed
5       destroyed
6       destroyed
*****************/