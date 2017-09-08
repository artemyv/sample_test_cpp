#include <boost/serialization/string.hpp>
#include <string>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using boost::property_tree::ptree; 
using boost::property_tree::write_json;

void create_array(ptree parent)
{
    std::stringstream oss;

    write_json(oss,parent);

    std::string serialized_strings(oss.str());

    std::cout << oss.str() << std::endl;
}
#if 0
int main()                                                                                                                                                                                                 
{   

    ptree parent;

    ptree pt,pt1;

    pt.put("prefix","standard");
    pt.put("faceID",42);
    //create_array(pt);
    pt1.put("prefix1","standard2");
    pt1.put("faceID2",44);
    //create_array(pt1); 
    parent.push_back(std::make_pair("",pt));
    parent.push_back(std::make_pair("",pt1));
    create_array(parent); 
}
#endif