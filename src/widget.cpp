//https://stackoverflow.com/questions/46203948/reading-array-of-the-json-in-jsoncpp/46205021#46205021
// Example program
    #include <iostream>
    #include <string>
    #include <json/json.h>
    int main()
    {

        std::string s = R"({
            "return":
            {
            "status":200,
            "message":"Accepted"
            },
            "entries":
            [
            {
            "messageid":185002992,
            "message":"CplusItsGood",
            "status":1,
            "statustext":"test",
            "sender":"1234567",
            "receptor":"123456789",
            "date":1234,
            "cost":140
            }
            ]
        })";


        Json::Reader reader;
        Json::Value root;

        reader.parse(s, root, false);

        auto entriesArray = root["entries"];

        auto firstelem = entriesArray[0];
        std::string sender = firstelem["sender"].asString();
        int i = std::stoi(sender);
        std::cout << "array:" << entriesArray << "\n";
        std::cout << "element:" << firstelem << "\n";
        std::cout << "value:" << sender << "\n";
        std::cout << "parsed value:" << i << "\n";
    }