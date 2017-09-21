#include <type_traits>

template <decltype(auto) RefOrValue>
struct Foo {
    static_assert(!std::is_reference_v<decltype(RefOrValue)>, "Should not be ref");
};

template <decltype(auto) RefOrValue>
struct Bar {
    static_assert(std::is_reference_v<decltype(RefOrValue)>, "Have to be ref");
    Foo<static_cast<std::decay_t<decltype(RefOrValue)>>(RefOrValue)> foo;
};


constexpr int value = 1;
int main() {
    Bar<(value)> bar;
    static_cast<void>(bar);
    Foo<value> foo;
}