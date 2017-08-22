// https://stackoverflow.com/questions/3207704/how-can-i-cin-and-cout-some-unicode-text

#include <iostream>
#include <locale>
#include <string>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.imbue(locale("en_US.UTF-8"));
    cout.imbue(locale("en_US.UTF-8"));

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