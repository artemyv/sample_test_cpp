#include <iostream>
#include <codecvt>
#include <locale>
#
#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#endif

int main()
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;

    std::string utf8 = "Testing unicode -- English -- Ελληνικά -- Español.";
    std::wstring ws = cvt.from_bytes(utf8);
    
#ifdef WIN32
	_setmode(_fileno(stdout), _O_U16TEXT);
#else
#endif
    std::cout << utf8 << '\n';
    std::wcout << ws << L'\n';

    std::cout << "Enter new string (utf8): ";
    getline(std::cin, utf8);
    ws = std::wstring_convert<std::codecvt_utf8<wchar_t>>{}.from_bytes(utf8);
    std::cout << utf8 << '\n';
    std::wcout << ws << L'\n';
    std::cin.clear();
    
    std::cout << "Enter new string (utf16): ";
    
    getline(std::wcin, ws);
    ws = cvt.from_bytes(utf8);
    utf8 = cvt.to_bytes(ws);
    std::cout << utf8 << '\n';
    std::wcout << ws << L'\n';

}