// https://stackoverflow.com/questions/45784065/calling-constexpr-function-in-enable-if-t
//works in gcc - shoudl not work in VS

#include <iostream>
#include <type_traits>

template <typename T, typename Enable = void>
struct A;

template <typename T>
constexpr bool f() { return true; }

template <typename T>
struct A<T, std::enable_if_t<f<T>()>> {};

int main() {
  A<int> f;

  std::cout << sizeof(f) << '\n';

}

/*************************************
Output
$ ./test
1
*************************************/