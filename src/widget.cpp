
#include <iostream>
#include <string>
#include <cstddef>



int year;
    int month;
    int day;

class Date{
    Date();
    ~Date();
    int getday();
    int getmonth();
    int getyear();
    void setday(int d);
    void setmonth(int m);
    void setyear(int y);
    std::string getdate();
}
;
Date::Date(){
    day = 1;
    month =1;
    year = 1900;

}
Date::~Date(){
    day = 1;
    month =1;
    year = 1900;

}

int Date::getday (){
    return day;
}
int Date::getmonth (){
    return month;
}
int Date::getyear (){
    return year;
}
void Date::setday(int d){
    day = d;
}
void Date::setmonth(int m){
    month = m;
}
void Date::setyear(int y){
    year = y;
}
std::string Date::getdate(){
    std::string completedate;
    std::string dayS;
    std::string monthS;
    std::string yearS;



    dayS = std::to_string(day);
    monthS=std::to_string(month);
    yearS=std::to_string(year);

    completedate = monthS + "/" + dayS ;
    completedate = completedate + "/";
    completedate = completedate + yearS;

    return completedate;// get the date
    }