#include <cstddef>

namespace vart {
namespace internal {

// destroy() destroys an object or a range
// of objects
//
template <class T>
void destroy( T* p )
{
    p->~T();
}

template <class FwdIter>
void destroy( FwdIter first, FwdIter last )
{
    while( first != last )
    {
        destroy( &*first );
        ++first;
    }
}

// swap() just exchanges two values
//
template <class T>
void swap( T& a, T& b )
{
    T temp(a);
    a = b;
    b = temp;
}
// construct() constructs a new object in
// a given location using an initial value
//
template <class T1, class T2>
void construct( T1* p, const T2& value )
{
    new (p) T1(value);
}

template <class T> class StackImpl
{
public:
    StackImpl(size_t size=0);
    StackImpl(const StackImpl& );
    ~StackImpl();
    void Swap(StackImpl& other) throw();
    size_t Count() const {
        return vused_;
    }
    void   Push(const T& val) ;
    T&     Top() ;   // if empty, throws exception
    void   Pop() ;   // if empty, throws exception

    T*     v_;      // ptr to a memory area big
    size_t vsize_;  //  enough for 'vsize_' T's
    size_t vused_;  // # of T's actually in use

private:
    // private and undefined: no copying allowed
    StackImpl& operator=( const StackImpl& );
};

template <class T>
StackImpl<T>::StackImpl( size_t size )
    : v_( static_cast<T*>
          ( size == 0
            ? 0
            : operator new(sizeof(T)*size) ) ),
      vsize_(size),
      vused_(0)
{
}

template <class T>
StackImpl<T>::StackImpl( const StackImpl<T>& other):
    v_( static_cast<T*>
        ( other.vsize_ == 0
          ? 0
          : operator new(sizeof(T)*other.vsize_) ) ),
    vsize_(other.vsize_),
    vused_(0)
{
    while( vused_ < other.vused_ )
    {
        construct( v_+vused_,
                   other.v_[vused_] );
        ++vused_;
    }
}

template <class T>
StackImpl<T>::~StackImpl()
{
    destroy( v_, v_+vused_ ); // this can't throw
    operator delete( v_ );
}

template <class T>
void StackImpl<T>::Swap(StackImpl& other) throw()
{
    internal::swap( v_,     other.v_ );
    internal::swap( vsize_, other.vsize_ );
    internal::swap( vused_, other.vused_ );
}

template <class T>
void StackImpl<T>::Push( const T& t )
{
    if( vused_ == vsize_ )  // grow if necessary
    {
        StackImpl temp( vsize_*2+1 );
        while( temp.Count() < vused_ )
        {
            temp.Push( v_[temp.Count()] );
        }
        temp.Push( t );
        Swap( temp );
    }
    else
    {
        construct( v_+vused_, t );
        ++vused_;
    }
}

template <class T>
T& StackImpl<T>::Top()
{
    if( vused_ == 0 )
    {
        throw "empty stack"; //never throw raw string
    }
    return v_[vused_-1];
}
template <class T>
void StackImpl<T>::Pop()
{
    if( vused_ == 0 )
    {
        throw "pop from empty stack";//never throw raw string
    }
    else
    {
        --vused_;
        destroy( v_+vused_ );
    }
}
}
}
