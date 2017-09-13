#include <iostream>
#include <codecvt>
#include <locale>
#include <type_traits>
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>

#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#endif

class MyCout
{
private:
	static std::once_flag flag1;
	bool bWide = false;
public:
	MyCout()
	{
		std::call_once(flag1, [](){ 
#ifdef WIN32
			//on windows - switch to utf-16, on linus the utf-8 is default - no need to switch
			_setmode(_fileno(stdout), MODETEXT);
			_setmode(_fileno(stdin), MODETEXT);
#endif
				});
#ifdef WIN32
		bWide = true;
#endif
	}
	std::stringstream s;

	template <typename T>
	MyCout& operator << (const T &x) {
		s << x;
		return *this;
	}

	~MyCout() {
		if(bWide){
			std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
			std::wcout << cvt.from_bytes( s.str());
		}
		else{
			std::cout << s.str();
		}
	}
};

//code below is the same for linux/windows
//linux part works out-of-the-box
//windows version requires following pre-sets
// 1. Store the source cpp file as UTF-8 BOM format before ccompiling
// 2. Change the font in the command line to one suporting unicode gliphs
//    Following answer worked for me https://stackoverflow.com/a/38694885/8491726

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
#if 0
int main()
{
	//internal string always utf-8
	std::string utf8 = u8"Testing unicode -- English -- 水 -- Ελληνικά -- Español -- Русский -- עיברית.";

	//when writing to stdout - optionally convert
	MyCout() << utf8  << '\n';
	MyCout() << "Enter new string: ";

	MyCout() << "test 1" << 1 << std::endl; 
	#if 0
	//when reading from stdin - use intemediate type
	tstring ws;
	getline(STDTCIN, ws);

	//optionally convert to inernal utf8 representation
	std::string user = convertFromStream(ws);

	//check what is being read
	STDTCOUT << convert2stream<tstring>(user + '\n');

	std::ofstream f1("test_utf8_direct.txt");

	f1 << utf8 << '\n' << user << '\n';
	f1.close();

	std::wofstream f2;
	std::locale l1;
	std::locale l2(l1, new std::codecvt_utf8<wchar_t>);
	f2.imbue(l2);
	f2.open("test_utf8_from16.txt");
	f2 << convert2stream<std::wstring>(utf8 + '\n');
	f2 << convert2stream<std::wstring>(user + '\n');
	#endif
}
/*************
Output
$ ./test
Testing unicode -- English -- 水 -- Ελληνικά -- Español -- Русский -- עיברית.
Enter new string: hello привет שלום
hello привет שלום


Actually hebrew text in the console is show left to right direction instad of correct right to left 
*/
#endif