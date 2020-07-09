#include "Parser.h"
#include <iostream>
using namespace std;

int main(int argc, char * argv[])
{   
    if(argc != 2)
    {
        cout << "usage: " << argv[0] << " filename" << endl;
        return 0;
    }
    else
    {
        Parser MyParser(argv[1]);     
    }

    return 0;
}
