// The C++ Programming Language
// Fourth Edition
// Bjarne Stroustrup
// 32.4.3 count() (§iso.25.2.9)
// x=count(b,e,v) x is the number of elements ∗p in [ b : e ) such that v==∗p
// x=count_if(b,e,v,f) x is the number of elements ∗p in [ b : e ) such that f(∗p)

//looks like we have a typo here - see sample below
//the second line shoudl be
// x=count_if(b,e,f) x is the number of elements ∗p in [ b : e ) such that f(∗p)

//sample also has to be modified to replace the direct call to isspace with lambda which 
//casts the passed parameter to unsigned char

#include <string>
#include <algorithm>
#include <iostream>
#include <cctype>
using namespace std;

auto f(const string& s)
{
    auto n_space = count(s.begin(),s.end(),' ');
    auto n_whitespace = count_if(s.begin(),s.end(),[](unsigned char c){ return std::isspace(c); });
    return make_pair(n_space, n_whitespace);
}

int main()
{
    std::string test("This is sample.\n");
    auto res = f(test);
    std::cout << "String \"" << test << "\" contains " << res.first << " spaces and " << res.second << " whitespaces.\n";
    return 0;

}

/*************************************
Output
$ ./test
String "This is sample.
" contains 2 spaces and 3 whitespaces.
*************************************/