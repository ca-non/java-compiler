#include <string>
using namespace std;

#ifndef ERROR_H
#define ERROR_H

class Error
{
public:
    enum Symbol {classt=0, publict, statict, voidt, maint, Stringt, extendst, returnt,
                 intt,booleant, ift, elset, whilet, Systemoutprintlnt, lengtht, truet,
                 falset, thist, newt, finalt, numt, idt,relopt, addopt, mulopt, signopt, assignopt, symbolt,
                 eoft, othert, unknownt, literalt, semit, commat, periodt, lparent, rparent,
                 lbrackt, rbrackt, lsbract, rsbract, quot, nott, writet, writelnt, readt};

    string keys[46] = {"class", "public", "static", "void", "main", "String", "extends", "return",
                 "int","boolean", "if", "else", "while", "System.out.println", "length", "true",
                 "false", "this", "new", "final", "num", "id","relop", "addop", "mulop", "signop","assignop", "symbol",
                 "eof", "other", "unknown", "literal", "semicolon", "comma", "period", "lparenthesis", "rparenthesis",
                 "lbracket", "rbracket", "lsbracket", "rsbracket", "quotation", "nott", "writet", "writelnt", "readt"};

/********************************************************************
**** FUNCTION    Error
*********************************************************************
****DESCRIPTION: constructor.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    Error();

/********************************************************************
**** FUNCTION    detect
*********************************************************************
****DESCRIPTION: displays a custom error message depending on the Flag.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void detect(int Flag, int LineNo, int s, int Token);

/********************************************************************
**** FUNCTION    ~Error
*********************************************************************
****DESCRIPTION: Destructor.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    ~Error();
};

#endif