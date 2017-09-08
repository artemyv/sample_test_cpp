
    #include<vector>
    #include<sstream> 

    std::vector<double> parse(const std::string& data)
    {
        std::istringstream ss(data);
        double val;
        std::vector<double> res;
        while(ss >> val){
            res.push_back(val);
        }
        return res;
    }
