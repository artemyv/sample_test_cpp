//https://stackoverflow.com/questions/45940074/finding-out-exit-index-point-in-the-matrix-for-a-dimension-mn-based-on-the-inde

#include <iostream>
#include <iomanip>
#include <complex>
#include <cmath>

constexpr auto xmax = 2;
constexpr auto ymax = 4;

int get(int Matrix[xmax][ymax], std::complex<int> pos)
{
    int x = pos.real();
    int y = pos.imag();
    int val = Matrix[x][y];
    Matrix[x][y] = 0;
    std::cout << "at " << x << 'x' << y << ' ' << val << '\n';
    return val;
}
int main()
{
    using namespace std::complex_literals;
    std::cout << std::fixed << std::setprecision(1);
 
    std::complex<int> direction(0,1);

    std::cout << "1st = " << direction << '\n';
    std::complex<int> turn(0,-1);
    direction*= turn;
    std::cout << "2nd = " << direction << '\n';
    direction*= turn;
    std::cout << "3nd = " << direction << '\n';
    direction*= turn;
    std::cout << "4th = " << direction << '\n';
    direction*= turn;
    std::cout << "5th = " << direction << '\n';
  
    int Matrix[xmax][ymax] = {{0,0,0,1}, {0,0,1,1}};

    for(auto x=0;x<xmax;x++)
    {
        for(auto y=0;y<ymax;y++){
            std::cout << "Set at " << x << 'x' << y << ' ' << Matrix[x][y] << '\n';
            
        }
    }
    std::complex<int> start(0,0);

    auto curr = start;
    for(;;)
    {
        auto val = get(Matrix,curr);
        if (val == 1) direction *= turn;
        auto next = curr + direction;
        if(next.real() <0 || next.real() >= xmax || next.imag() < 0 || next.imag() >= ymax){
            break;
        }
        curr = next;
    }
    std::cout << "exit " << curr.real() << 'x' << curr.imag() << '\n';
    
}