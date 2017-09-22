//Sutter H. - More Exceptional C++ (C++ In-Depth) - 2001
//Item 32. Recursive Declarations

class FuncPtr_;
typedef FuncPtr_ (*FuncPtr)();
class FuncPtr_
{
public:
    FuncPtr_( FuncPtr p ) : p_( p ) { }
    operator FuncPtr() {
        return p_;
    }
private:
    FuncPtr p_;
};


FuncPtr_ f() {
    return f;    // natural return syntax
}

int main()
{
    FuncPtr p = f();  // natural usage syntax
    p();
}
