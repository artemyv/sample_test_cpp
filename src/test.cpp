//https://stackoverflow.com/questions/45812887/getting-the-1st-input-type-of-a-function/45814006#45814006
//my accepted response

#include <vector>

struct List {
};

struct Data {
    void* ptr_value;
};
struct Node {
    Node* next;
    Data data;
};
Node* list_head(const List*const)
{
    return nullptr;
}

template<typename T, typename inputtype,  typename ...Params>
using Fn = T (*) (inputtype*, Params...);

template<typename T, typename inputtype,  typename ...Params, typename = Fn<T,inputtype, Params...>>
inline auto ListMap(List* polymorphic_list, Fn<T,inputtype, Params...> f,Params&&... params) {

    std::vector<T> res;
    for (auto lcell = list_head(polymorphic_list); lcell; lcell = lcell->next) {
        const auto v = lcell->data.ptr_value;
        inputtype* in = static_cast<inputtype*>(v);
        res.emplace_back(std::move(f(in, std::forward<Params>(params)...)));
    }
    return res;
}

int f(void*,int& x)
{
    return x;
}
int main()
{
    int x = 1;
    auto res = ListMap(new List(),f,x);
}

