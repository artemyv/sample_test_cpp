//https://stackoverflow.com/questions/45964077/thread-inside-a-class-with-member-function-from-another-class

#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <mutex>

using namespace std::literals::chrono_literals;
std::mutex mtx;

void putime(std::ostream& s){
    auto t = std::chrono::high_resolution_clock::now();
    auto dur =  t.time_since_epoch();
    auto int_ms = std::chrono::duration_cast<std::chrono::microseconds>(dur);
    auto ns = int_ms.count();
    s << ns/1000000 << "." << std::setfill('0') << std::setw(6) << ns%1000000 << "s: ";
}

void log(std::string slog){
    {
        std::hash<std::thread::id> hasher;
        
        std::lock_guard<std::mutex> g {mtx};
        putime(std::cout);
        std::cout << slog << ' ' << hasher(std::this_thread::get_id()) %100 << '\n' ;
    }
}
class Boo
{
public:
    void run()
    {
        log( "Boo::run started");

        std::this_thread::sleep_for(5s);

        log( "Boo::run finished");
    }
};

class Foo
{
public:
    void run()
    {
        log( "Foo::run started");

        t1 = std::thread(&Boo::run,boo);   // threads already default constructed
        t2 = std::thread(&Foo::user,this); // so need to *assign* them

        log( "Foo::run finihsed");
    }
    void user()
    {
        log( "Foo::user started");
        
        std::this_thread::sleep_for(5s);

        log( "Foo::user finihsed");
    }

    ~Foo() {
        t1.join();
        t2.join();
        log( "Foo::~Foo finihsed");
    }
private:
    std::thread t1;
    std::thread t2;
    Boo boo;
};
int main()
{
    log( "main started");
    
    Foo foo;
    foo.run();

    log( "main finihsed");
}