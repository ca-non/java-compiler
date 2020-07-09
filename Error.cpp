#include "Error.h"
#include <iostream>
using namespace std;

/********************************************************************
**** FUNCTION    Error
*********************************************************************
****DESCRIPTION: constructor.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
Error::Error()
{}

/********************************************************************
**** FUNCTION    detect
*********************************************************************
****DESCRIPTION: displays a custom error message depending on the Flag.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Error::detect(int Flag, int LineNo, int s, int Token)
{
    if(Flag == 1)
    {
        cout << "ERROR: Line " << LineNo << " Invalid Token expecting " << keys[s]
             << " found "<< keys[Token] << endl;
        exit(1);
    }
    else if(Flag == 2)
    {
        cout << "ERROR: Line " << LineNo << " Invalid Token expecting '{' or 'extends' found "
             << keys[Token]<< endl;
        exit(1);
    }
    else if(Flag == 3)
    {
        cout << "ERROR: Line " << LineNo << " Invalid Token expecting 'identifier ,' or 'identifier ;' found "
             << keys[Token] << endl;
        exit(1);
    }
    else if(Flag == 4)
    {
        cout << "ERROR: Line " << LineNo << " Invalid Token expecting 'int, boolean or void' found "
             << keys[Token]  << endl;
        exit(1);
    }
}

/********************************************************************
**** FUNCTION    ~Error
*********************************************************************
****DESCRIPTION: Destructor.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
Error::~Error()
{}