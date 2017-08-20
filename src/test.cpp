// The C++ Programming Language
// Fourth Edition
// Bjarne Stroustrup
// 32.4.1 for_each()
// f=for_each(b,e ,f) (§iso.25.2.4) Do f(x) for each x in [ b : e ); return f

//I was interested why you need return value - so I read following blog
//http://xenon.arcticus.com/c-morsels-std-for-each-functors-member-variables



#include <vector>
#include <iostream>
#include <algorithm>

class Contained
{
public:
    Contained() : _shouldBeCounted(0)
    {

    }
    bool getShouldBeCounted(void) const
    {
        return(_shouldBeCounted);
    }
    void setShouldBeCounted(bool newShouldBeCounted)
    {
        _shouldBeCounted = newShouldBeCounted;
    }
private:
    bool _shouldBeCounted;
};

class CountingFunctor
{
public:
    CountingFunctor() : _counter(0)
    {

    }
    int getCounter(void) const
    {
        return(_counter);
    }
    void operator () (Contained item)
    {
        if(item.getShouldBeCounted()) _counter++;
    }
private:
    int _counter;
};

int main(void)
{
    std::vector<Contained> v(10);
    for(std::vector<Contained>::size_type i=0; i<v.size(); ++i) v[i].setShouldBeCounted(i%2==0);

    CountingFunctor f1;
    CountingFunctor f2=std::for_each(v.begin(),v.end(),f1);
    std::cout << "f1.getCounter()==" << f1.getCounter() << std::endl;
    std::cout << "f2.getCounter()==" << f2.getCounter() << std::endl;

    return 0;
}

/******************
 Output:
 $ ./test
f1.getCounter()==0
f2.getCounter()==5
*******************/