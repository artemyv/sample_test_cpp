//Testing Item 12: Declare overriding functions override .
//From Effective Modern C++
//  42 SPECIFIC WAYS TO IMPROVE YOUR USE OF C++11 AND C++14
//  by Scott Meyers

class Base {
public:
virtual void mf1() const;
virtual void mf2(int x);
virtual void mf3() &;
void mf4() const;
};

//compiled without warnings in gcc (g++ (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609)
class Derived: public Base {
public:
virtual void mf1();
virtual void mf2(unsigned int x);
virtual void mf3() &&;
void mf4() const;
};

//all 4 cases give warnings in gcc (g++ (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609)
class Derived2: public Base {
public:
virtual void mf1() override;
virtual void mf2(unsigned int x) override;
virtual void mf3() && override;
void mf4() const override;
};
int main() {
    return 0;
}
