// The C++ Programming Language
// Fourth Edition
// Bjarne Stroustrup
// 32.4.4 find() (§iso.25.2.5)
// p=find(b,e,v) points to the first element in [ b : e ) such that ∗p==v
// p=find_if(b,e,f) points to the first element in [ b : e ) such that f(∗p)
// p=find_if_not(b,e,f) p points to the first element in [ b : e ) such that !f(∗p)
// p=find_first_of(b,e ,b2,e2) p points to the first element in [ b : e ) such that ∗p==∗q for some q in [ b2 : e2 )
// p=find_first_of(b,e ,b2,e2,f) p points to the first element in [ b : e ) such that f(∗p,∗q) for some q in [ b2 : e2 )
// p=adjacent_find(b,e) p points to the first element in [ b : e ) such that ∗p==∗(p+1)
// p=adjacent_find(b,e,f) p points to the first element in [ b : e ) such that f(∗p,∗(p+1))
// p=find_end(b,e,b2,e2) p points to the last ∗p in [ b : e ) such that ∗p==∗q for an element ∗q in [ b2 : e2 )
// p=find_end( b,e,b2,e2,f) p points to the last ∗p in [ b : e ) such that f(∗p,∗q) for an element ∗q in [ b2 : e2 )

#include <string>
#include <algorithm>
#include <iostream>
#include <cctype>
void f(const std::string& s)
{
    auto p_space = find(s.begin(),s.end(),' ');

    std::cout << "Space is at " << std::distance(s.begin(),p_space) << '\n';
    auto p_space2 = find(s.rbegin(),s.rend(),' ');
    std::cout << "Last Space is at " << std::distance(p_space2,s.rend())-1 << '\n';
    auto p_whitespace = find_if(s.begin(),s.end(), isspace);
    std::cout << "Witespace is at " << std::distance(s.begin(),p_whitespace)  << '\n';

    //std::isspace will not work... 
    auto p_whitespace2 = find_if(s.rbegin(),s.rend(), isspace);
    std::cout << "Last Witespace is at " << std::distance(p_whitespace2,s.rend())-1 << '\n';


    std::vector<char> seps{',','.'};

    auto p_sep = find_first_of(s.begin(),s.end(),seps.begin(),seps.end());
    std::cout << "First Separator is at " << std::distance(s.begin(),p_sep)  << '\n';
    auto word = find_if_not(s.begin(),s.end(),isalpha);
    std::cout << "Word ends at " << std::distance(s.begin(),word) << '\n';
}

int main()
{
    std::string test("test 5 7,9.b\n");
    f(test);
    return 0;

}

/*************************************
Output
$ ./test
Space is at 4
Last Space is at 6
Witespace is at 4
Last Witespace is at 12
First Separator is at 8
Word ends at 4
*************************************/