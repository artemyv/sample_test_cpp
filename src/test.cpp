#include <Windows.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ostream>
#include <iterator>

using namespace std;

vector<wstring> locals;

BOOL CALLBACK MyFuncLocaleEx(LPWSTR pStr, DWORD dwFlags, LPARAM lparam)
{
    locals.push_back(pStr);
    return TRUE;
}

int _tmain(int argc, _TCHAR* argv[])
{
    EnumSystemLocalesEx(MyFuncLocaleEx, LOCALE_ALL, NULL, NULL);

    for (vector<wstring>::const_iterator str = locals.begin(); str != locals.end(); ++str)
        wcout << *str << endl;

    wcout << "Total " << locals.size() << " locals found." << endl;

    return 0;
}

#if 0

// https://stackoverflow.com/questions/3207704/how-can-i-cin-and-cout-some-unicode-text

#include <iostream>
#include <locale>
#include <string>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    locale::global(locale("en_US.UTF-8"));

    string s;
    string t(" la Polynésie française");

    cin >> s;
    cout << s << t << endl;
    return 0;
}

/*********************************
Output
$ ./test
ntcтестמאבמ
ntcтестמאבמ la Polynésie française
*********************************/

#endif