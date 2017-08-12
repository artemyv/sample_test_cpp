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
