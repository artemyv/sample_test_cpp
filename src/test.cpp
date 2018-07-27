#include <iostream>
#include <functional>

void g(int)
{
	std::cout << "g OK\n";
}

int h(int u)
{
	std::cout << "h OK\n";
	return u;
}

template <class T, class U>
T f(std::function<T(U)> g, U u, T defaultval = T())
{
	T res = g(u);
	return defaultval;
}

template <class U>
void f(std::function<void(U)> g, U u)
{
	return g(u);
}

int main()
{
	f(std::function<void(int)>(g), 123);
	f(std::function<int(int)>(h),123, 0);

	return 0;
}
