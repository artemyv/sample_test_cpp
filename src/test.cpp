//https://stackoverflow.com/questions/45930117/reading-from-a-file-c

    #include <iostream>
    #include <fstream>
    #include <string>
    #include <vector>

    std::vector<std::string> readfile(std::istream& infile)
    {
        //creating array and populating with list of words
        std::string word;
        std::vector<std::string> arr;

        while (getline(infile, word))
            arr.push_back(std::move(word));

        return arr;
    }
    int main() {

        std::string filename{"test.txt"};

        std::ifstream infile(filename);
        if (!infile)
        {
            std::cout << "File could not be found!\n";
            return -1;
        }

        auto res = readfile(infile);

        //testing
        for(auto& s:res)
            std::cout << s << '\n';

        return 0;
    }