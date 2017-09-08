#include <iostream>
#include <codecvt>
#include <locale>
#include <type_traits>

#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#define STDTCOUT std::wcout
#define STDTCIN std::wcin
using tstring = std::wstring;
#else
#define STDTCOUT std::cout
#define STDTCIN std::cin
using tstring = std::string;
#endif

//version for utf8 stdout
template<class T>
T convert2stream(std::string source, typename std::enable_if<std::is_same<T, std::string>::value >::type* = 0)
{
    return source;
}


//version for utf16 stdout
template<class T>
T convert2stream(std::string source, typename std::enable_if<std::is_same<T, std::wstring>::value >::type* = 0)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
	return cvt.from_bytes(source);
}

//version for utf8 stdin
template<class T>
std::string convertFromStream(T source, typename std::enable_if<std::is_same<T, std::string>::value >::type* = 0)
{
    return source;
}

//version for utf16 stdin
template<class T>
std::string convertFromStream(T source, typename std::enable_if<std::is_same<T, std::wstring>::value >::type* = 0)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
	return cvt.from_bytes(source);
}

int main()
{
#ifdef WIN32
	//on windows - switch to utf-16, on linus the utf-8 is default - no need to switch
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
#endif

	//internal string always utf-8
	std::string utf8 = u8"Testing unicode -- English -- Ελληνικά -- Español.";

	//when writing to stdout - optionally convert
	STDTCOUT << convert2stream<tstring>(utf8 + '\n');
	STDTCOUT << convert2stream<tstring>("Enter new string: ");

	//when reading from stdin - use intemediate type
	tstring ws;
	getline(STDTCIN, ws);

	//optionally convert to inernal utf8 representation
	utf8 = convertFromStream(ws);

	//check what is being read
	STDTCOUT << convert2stream<tstring>(utf8 + '\n');

}