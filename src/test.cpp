//https://stackoverflow.com/questions/45892146/max-and-min-sum#comment78743531_45892146

#include<iostream>
using namespace std;
int main()
{
    long a[5],max=0,min=10000,sum;
    int i,j;
    for(i=0; i<5; i++)
        cin>>a[i];

    for(j=0; j<5; j++)
    {
        sum=0;
        for(i=0; i<5; i++)
        {

            if(i!=j)
            {
                sum=sum+a[i];
            }
        }
        if(sum>max)
            max=sum;
        if(sum< min)
            min=sum;
    }
    cout<<min<<" "<<max;
    return 0;
}

/*******************
Answer

It looks like you are having overflow issues. The max value for long type is 2,147,483,647. Adding up all the values you entered gives 3,001,208,382. Try changing

long a[5], max=0, min=10000, sum; 

to

double a[5], max=0, min=10000, sum; 

and see if you get correct values.
*/