#include "widget.h"
#include <string>
#include <vector>

// in "widget.cpp"
struct Widget::Impl {
    std::string name;
    std::vector<double> data;
}; // as before

Widget::Widget()
    : pImpl(std::make_unique<Impl>())
{}

Widget::~Widget() =default;
Widget::Widget(Widget&& ) = default;
Widget& Widget::operator=(Widget&& ) = default;

Widget::Widget(const Widget& rhs)
    : pImpl(std::make_unique<Impl>(*rhs.pImpl))
{
// copy ctor
}

Widget& Widget::operator=(const Widget& rhs)
{
    *pImpl = *rhs.pImpl;
    return *this;
}
