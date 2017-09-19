#include <iostream>
#include <thread>
#include <vector>

using namespace std;

class run {
public:
    run()
    : lifetime("constructed")
    { 
        cout << lifetime << endl; 
    }

    run(const run& other)
    : lifetime("copied from " + other.lifetime)
    { 
        cout << lifetime << endl; 
    }
    run(run&& other) 
    : lifetime("move-constructed from " + other.lifetime)
    { 
        other.lifetime = "[zombie] - " + other.lifetime;
        cout << lifetime << endl; 
    }
    run& operator=(const run& other) 
    { 
        lifetime = "copy assigned from " + other.lifetime + ", was once " + lifetime; 
        cout << lifetime << endl; 
        return *this; 
    }

    run& operator=(run &&other) 
    { 
        lifetime = "move-assigned from " + other.lifetime + ", was once " + lifetime; 
        other.lifetime = "[zombie] - " + other.lifetime;
        cout << lifetime << endl; 
        return *this; 
    }

    ~run() 
    {
        lifetime = "lifetime ending: " + lifetime;
        cout << lifetime << endl; 
    }

    void operator()() {};

    std::string lifetime;    
};

int main() {
    run thread_r;
    thread t(thread_r);
    t.join();
}