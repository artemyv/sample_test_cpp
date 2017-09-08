#include <iostream>
#include <codecvt>
#include <locale>
#include <type_traits>

#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#define WIDE
#endif

#ifdef WIDE
#define STDTCOUT std::wcout
#define STDTCIN std::wcin
using tstring = std::wstring;
#define MODETEXT _O_U16TEXT 
#else
#define STDTCOUT std::cout
#define STDTCIN std::cin
using tstring = std::string;
#define MODETEXT _O_U8TEXT 
#endif

void initconsole()
{
#ifdef WIN32
	//on windows - switch to utf-16, on linus the utf-8 is default - no need to switch
	_setmode(_fileno(stdout), MODETEXT);
	_setmode(_fileno(stdin), MODETEXT);
#endif
}


//code below is the same for linux/windows
//linux part works out-of-the-box
//windows version requires following pre-sets
// 1. Store the source cpp file as UTF-8 BOM format before ccompiling
// 2. Change the font in the command line to one suporting unicode gliphs
//    

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
	return cvt.to_bytes(source);
}

int main()
{
	initconsole();

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
