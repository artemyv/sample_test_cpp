//https://stackoverflow.com/questions/46003624/exception-handling-in-c-4

#include<iostream>
using namespace std;

class A {
public:
    A() { cout << "default\n"; }
    A(const A&) { cout << "copy\n"; }
    A(A&&) { cout << "move\n"; }
    ~A() { cout << "~\n"; }
};

int main()
{
    try {
       A obj;
       throw obj;
    } catch(const A& a) {
       cout<<"Caught\n";
    }
}

/*****************
Output 
$ ./test
default
move
~
Caught
~
*/

/*****************
Output when caught by value instead
$ ./test
default
move
~
copy
Caught
~
~
*/