// https://stackoverflow.com/questions/45783833/c-functor-template-for-class-member-functions/45785878#45785878

template<class>
struct Functor;

template<class TReturn, class... TParameter>
struct Functor<TReturn(TParameter...)> {
    TReturn (*ptr)(void*, TParameter...);
    void     *object;

    template<class TObject, TReturn(TObject::*TMemberFunction)(TParameter...)>
    static TReturn memberCaller(void *obj, TParameter... params) {
        TObject *c = static_cast<TObject*>(obj);
        return (c->*TMemberFunction)(params...);
    }

    TReturn operator()(TParameter... params) {
        return ptr(object, params...);
    }
};

#include <iostream>
class Test {
public:
    void func(int a) {
        std::cout << a << std::endl;
    }
};
int main()
{
    Functor<void(int)> f;
    Test               t;
    
    f.object = &t;
    f.ptr    = &Functor<void(int)>::memberCaller<Test, &Test::func>;
    
    f(100);
    
}
/*************************************
Output
$ ./test
100
*************************************/