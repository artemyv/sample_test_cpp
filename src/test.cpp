//https://stackoverflow.com/questions/45967321/a-class-to-be-accessed-by-another-class-only/45967426#45967426

#include <iostream>

class B
{
    class A
    {
    private:
        static int x;
    public:
        static int y;
        static int getX() {
            return A::x;
        }
        static void setX(int z) {
            A::x = z;
        }
    };

public:
    void showX() {
        A::setX(9) ;
        std::cout << A::getX() << std::endl;
    }
    void showY() {
        A::y = 8;
        std::cout << A::y << std::endl;
    }
};

int B::A::x = 0;
int B::A::y = 0;

int main()
{
    B b1;
    b1.showX();
    b1.showY();
}