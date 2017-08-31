//https://stackoverflow.com/questions/45973779/creating-an-array-of-char-based-on-user-input

    #include <iostream>
    #include <string>
    #include <algorithm>
    #include <cctype>

    //pass parameter by value so changes to wordA will not modify original string
    bool checkPalindrome(std::string wordA) {

        //removing whitespaces and punctuation
        wordA.erase (remove_if(wordA.begin(), wordA.end(), [](unsigned char c) {
            return std::isspace(c) || std::ispunct(c);
        }), wordA.end());

        //changing the string to lower case
        std::transform(wordA.begin(), wordA.end(), wordA.begin(),::tolower);

        //creating copy of the string
        auto wordB = wordA;

        //function name is self-explanary
        std::reverse(wordB.begin(), wordB.end());

        return (wordB == wordA);
    }

    int main()
    {
        std::cout << "The word is:\n";

        std::string word;
        getline(std::cin, word);

        if(checkPalindrome(word))
            std::cout << "It is a palindrome\n";
        else
            std::cout << "It is NOT a palindrome\n";

        return 0;
    }

    /*************
    Output
    $ ./test
    The word is:
    A car, a man, a maraca.
    It is a palindrome

    */