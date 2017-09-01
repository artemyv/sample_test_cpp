//https://stackoverflow.com/questions/46006690/how-to-use-variadic-function-with-strings-without-using-templates

//sample using initializer list
#include <string>
#include <iostream>

void update( std::initializer_list< std::string > args )
{
    std::string prefix = "";
    for(auto v:args){
        std::cout << prefix << v;
        prefix = ", ";
    }
    std::cout << '\n';
}

int main() {
    update({"1", "2", "3"});
}