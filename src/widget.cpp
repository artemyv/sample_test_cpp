//https://stackoverflow.com/questions/46317751/utf-8-sprintf-strlen-etc#46318241

#include <iostream>
#include <codecvt>
#include <string>
#include <locale>

std::string cutString(const std::string& in, size_t len)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    auto wstring = cvt.from_bytes(in);
    if(len < wstring.length())
    {
        wstring = wstring.substr(len);
        return cvt.to_bytes(wstring);
    }    
    return in;
}
int main(){
    std::string test = "你好世界這是演示樣本";

    std::string res = cutString(test,5);
    std::cout << res << '\n';
    
    return 0;
}

/****************
Output 
$ ./test
是演示樣本
*/