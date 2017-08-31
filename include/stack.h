#include "stackimpl.h"
namespace vart{
    template <class T> 
    class Stack
    {
    public:
      Stack(size_t size=0);
      ~Stack() noexcept {};
      Stack(const Stack&);
      Stack& operator=(const Stack&);
      size_t Count() const { return impl_.Count();}
      void   Push(const T& val) {impl_.Push(val);}
      T&     Top() {return impl_.Top();}   // if empty, throws exception
      void   Pop() {impl_.Pop();}   // if empty, throws exception
    private:
      internal::StackImpl<T> impl_;  // private implementation

      void Swap (Stack& ) noexcept;
    };
}


template <class T> 
vart::Stack<T>::Stack(size_t size):
impl_(size)
{
}

template <class T> 
vart::Stack<T>::Stack(const vart::Stack<T>& other):
impl_(other.impl_)
{


}

template <class T> 
vart::Stack<T>& vart::Stack<T>::operator=(const vart::Stack<T>& other)
{
    Stack<T> temp(other);
    Swap(temp);
    return *this;
}

template <class T> 
void vart::Stack<T>::Swap(vart::Stack<T>& other) noexcept
{
    impl_.Swap(other.impl_);
}
