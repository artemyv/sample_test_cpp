//Exceptionaal C++. Sample2. Case insensitive string

#include <string>
#include <iostream>
#include <cstring>

struct ci_char_traits : public std::char_traits<char>
// just inherit all the other functions
//  that we don't need to replace
{
    static bool eq( char c1, char c2 )
    {
        return toupper(c1) == toupper(c2);
    }
    static bool lt( char c1, char c2 )
    {
        return toupper(c1) <  toupper(c2);
    }
    static int compare( const char* s1,
                        const char* s2,
                        size_t n )
    {
        while(n>0){
            if(!eq(*s1, *s2))
                return *s1 - *s2;
            if(*s1 == 0)
                return 0;
            s1++;
            s2++;
            n--;
        }
        return 0;
    }
// if available on your platform,
//  otherwise you can roll your own
    static const char*
    find( const char* s, int n, char a )
    {
        while( n-- > 0 && toupper(*s) != toupper(a) )
        {
            ++s;
        }
        return n >= 0 ? s : 0;
    }
};

typedef std::basic_string<char, ci_char_traits> ci_string; 

int main()
{
    ci_string s( "AbCdE" ); 
    // case insensitive
    //
    if( s == "abcde" )
        std::cout << "1 good\n";
    else
        std::cout << "1 bad\n";
    
    if( s == "ABCDE" )
        std::cout << "2 good\n";
    else
        std::cout << "2 bad\n";
    // still case-preserving, of course
    //
    if( strcmp( s.c_str(), "AbCdE" ) == 0 )
        std::cout << "3 good\n";
    else
        std::cout << "3 bad\n";
    if( strcmp( s.c_str(), "abcde" ) != 0 )
        std::cout << "4 good\n";
    else
        std::cout << "4 bad\n";

    if( s < "ABCDF" )
        std::cout << "5 good\n";
    else
        std::cout << "5 bad\n";

}