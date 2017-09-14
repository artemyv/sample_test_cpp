//https://stackoverflow.com/questions/46209857/what-happened-to-operator/46210402#46210402
// Example program
    #include <iostream>
    #include <string>
    int main()
    {

        unsigned int res = 0;
        signed char val = -64;
        res += val;

        std::cout << res << '\n';
        
        res = 0;
        res += static_cast<unsigned char>(val);
        std::cout << res << '\n';
        

    }

    /****************
    Output
    $ ./test
    4294967232
    192
    */