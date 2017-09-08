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

tstring convert2stream(std::string source) {
#ifdef WIN32
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
	return cvt.from_bytes(source);
#else
	return source;
#endif
}

std::string convertFromStream(tstring source) {
#ifdef WIN32
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
	return cvt.to_bytes(source);
#else
	return source;
#endif
}


int main()
{
#ifdef WIN32
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
#endif

	std::string utf8 = u8"Testing unicode -- English -- Ελληνικά -- Español.";
	STDTCOUT << convert2stream(utf8 + '\n');
	STDTCOUT << convert2stream("Enter new string: ");

	tstring ws;
	getline(STDTCIN, ws);

	utf8 = convertFromStream(ws);
	STDTCOUT << convert2stream(utf8 + '\n');

}