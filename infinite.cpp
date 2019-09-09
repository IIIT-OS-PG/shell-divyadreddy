#include<iostream>
#include<unistd.h>

using namespace std;

int main()
{
    while(1)
    {
        sleep(5);
        cout<<"hey!"<<endl;
    }
    return 0;
}