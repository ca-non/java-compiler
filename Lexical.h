#include <string>
#include <fstream>
using namespace std;

#ifndef LEXICAL_H
#define LEXICAL_H

class Lexical
{
private:
    ifstream inFile;
public:
    enum Symbol {classt=0, publict, statict, voidt, maint, Stringt, extendst, returnt,
                 intt,booleant, ift, elset, whilet, Systemoutprintlnt, lengtht, truet,
                 falset, thist, newt, finalt, numt, idt,relopt, addopt, mulopt, signopt, assignopt, symbolt,
                 eoft, othert, unknownt, literalt, semit, commat, periodt, lparent, rparent,
                 lbrackt, rbrackt, lsbract, rsbract, quot, nott, writet, writelnt, readt};

    Symbol Token;
    string Lexeme;
    char ch;
    int Value;
    string  ValueR;
    string Literal;
    string resWords[46];
    bool iflag;
    int LineNo;


/********************************************************************
**** FUNCTION    Lexical
*********************************************************************
****DESCRIPTION: constructor.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    Lexical();

/********************************************************************
**** FUNCTION    Lexical
*********************************************************************
****DESCRIPTION: overloaded constructor.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    Lexical(char[]);

/********************************************************************
**** FUNCTION    Initiate
*********************************************************************
****DESCRIPTION:Initialize the Lexical analyser with the filename.
****INPUT ARGS :char *fileName
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void Initiate(char *fileName);

/********************************************************************
**** FUNCTION    GetNextToken
*********************************************************************
****DESCRIPTION:Get the next token.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void GetNextToken();

/********************************************************************
**** FUNCTION    DisplayToken
*********************************************************************
****DESCRIPTION:Display the current token
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void DisplayToken();

/********************************************************************
**** FUNCTION    GetNextCh
*********************************************************************
****DESCRIPTION:Get the character from the stream.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void GetNextCh();

/********************************************************************
**** FUNCTION    ProcessToken
*********************************************************************
****DESCRIPTION:Process tokens.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void ProcessToken();

/********************************************************************
**** FUNCTION    ProcessWordToken
*********************************************************************
****DESCRIPTION:Process alphabetic tokens.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void ProcessWordToken();

/********************************************************************
**** FUNCTION    ProcessNumToken
*********************************************************************
****DESCRIPTION:Process numeric tokens.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void ProcessNumToken();

/********************************************************************
**** FUNCTION    ProcessComment
*********************************************************************
****DESCRIPTION:Process single/double comments.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void ProcessComment();

/********************************************************************
**** FUNCTION    ProcessLiteral
*********************************************************************
****DESCRIPTION:Process literals.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void ProcessLiteral();

/********************************************************************
**** FUNCTION    ProcessDoubleToken
*********************************************************************
****DESCRIPTION:Process double tokens.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void ProcessDoubleToken();

/********************************************************************
**** FUNCTION    ProcessSingleToken
*********************************************************************
****DESCRIPTION:Process a single token.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void ProcessSingleToken();

/********************************************************************
**** FUNCTION    ProcessAddop
*********************************************************************
****DESCRIPTION:Process add operator single/double.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void ProcessAddop();

/********************************************************************
**** FUNCTION    ProcessMulop
*********************************************************************
****DESCRIPTION:Process mul operator single/double.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void ProcessMulop();

/********************************************************************
**** FUNCTION   ProcessDoubleAddop
*********************************************************************
****DESCRIPTION:Process double add operators.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void ProcessDoubleAddop();

/********************************************************************
**** FUNCTION    ProcessDoubleMulop
*********************************************************************
****DESCRIPTION:Process double mul operators.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void ProcessDoubleMulop();

/********************************************************************
**** FUNCTION    ProcessOther
*********************************************************************
****DESCRIPTION:Process other tokens.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/  
    void ProcessOther(); 

/********************************************************************
**** FUNCTION     ~Lexical
*********************************************************************
****DESCRIPTION: Destructor.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    ~Lexical();
};

#endif