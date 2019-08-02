#include <iostream>

template<typename... Ts> void func(Ts... args){
    int dummy[sizeof...(Ts)] = { (std::cout << args, 0)... };
}

template<class T>
auto wrapper(T&& arg) 
{
    // arg is always lvalue
    return std::forward<T>(arg); // Forward as lvalue or as rvalue, depending on T
}

auto wrapper(const wchar_t* arg) 
{
    // arg is always lvalue
    return "converted"; 
}
template<typename... Ts> void orig(Ts... args){
    func(wrapper(args)...);
}

int main()
{
	orig("test", L"test2", 1,2.4);

	return 0;
}
