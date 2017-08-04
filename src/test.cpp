<<<<<<< HEAD
//Declare overriding functions override.
//Testing Item 12: From Effective Modern C++
=======
//Testing Item 12: Declare overriding functions override .
//From Effective Modern C++
>>>>>>> 43650921efb1d5404dcadc5b9bb75050aa22e0b8
//  42 SPECIFIC WAYS TO IMPROVE YOUR USE OF C++11 AND C++14
//  by Scott Meyers

class Base {
public:
<<<<<<< HEAD
    virtual void mf1() const;
    virtual void mf2(int x);
    virtual void mf3() &;
    void mf4() const;
=======
virtual void mf1() const;
virtual void mf2(int x);
virtual void mf3() &;
void mf4() const;
>>>>>>> 43650921efb1d5404dcadc5b9bb75050aa22e0b8
};

//compiled without warnings in gcc (g++ (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609)
class Derived: public Base {
public:
<<<<<<< HEAD
    virtual void mf1();
    virtual void mf2(unsigned int x);
    virtual void mf3() &&;
    void mf4() const;
=======
virtual void mf1();
virtual void mf2(unsigned int x);
virtual void mf3() &&;
void mf4() const;
>>>>>>> 43650921efb1d5404dcadc5b9bb75050aa22e0b8
};

//all 4 cases give warnings in gcc (g++ (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609)
class Derived2: public Base {
public:
<<<<<<< HEAD
    virtual void mf1() override;
    virtual void mf2(unsigned int x) override;
    virtual void mf3() && override;
    void mf4() const override;
=======
virtual void mf1() override;
virtual void mf2(unsigned int x) override;
virtual void mf3() && override;
void mf4() const override;
>>>>>>> 43650921efb1d5404dcadc5b9bb75050aa22e0b8
};
int main() {
    return 0;
}
