#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>

template<class T, class Enable = void>
struct RealNumber
{
	RealNumber(double val)
		: integer(static_cast<int>(val)),
		  fraction(static_cast<short>((val - integer)*100))
	{}

	int integer;
	short fraction;
};

template<class T>
struct RealNumber<T, typename std::enable_if_t<std::is_floating_point<T>::value>>
{
	RealNumber(T val)
		: value(val)
	{}

	T value;
};

int main()
{

	auto a = RealNumber<double>(1.3);
	auto b = RealNumber<float>(1.3f);
	auto c = RealNumber<long double>(1.3l);
	auto d = RealNumber<int>(1);
	auto e = RealNumber<int>(1.5);

	std::cout << a.value << std::endl;
	std::cout << b.value << std::endl;
	std::cout << c.value << std::endl;
	std::cout << d.integer << '.' << d.fraction << std::endl;
	std::cout << e.integer << '.' << e.fraction << std::endl;

	return 0;
}
