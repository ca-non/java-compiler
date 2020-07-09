#include "Lexical.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
using namespace std;

/********************************************************************
**** FUNCTION    Lexical
*********************************************************************
****DESCRIPTION: constructor.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
Lexical::Lexical()
{}

/********************************************************************
**** FUNCTION    Initiate
*********************************************************************
****DESCRIPTION:Initialize the Lexical analyser with the filename.
****INPUT ARGS :char *fileName
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::Initiate(char *fileName)
{
    Token = unknownt;
    ch = ' ';
    LineNo = 0;

    inFile.open(fileName);

    if(!inFile)
    {
        cout << "Entered file does not exist." << endl;
        exit(99);
    }

    string res[46] = {"class", "public", "static", "void", "main", "String", "extends", "return",
                "int","boolean", "if", "else", "while", "System.out.println", "length", 
               "true","false", "this", "new", "final", "num", "id", "relop", "addop","mulop",
                "signop","assignop", "symbol", "eof", "other", "unknown", "literal", "semi",
                "comma", "period", "lparen", "rparen", "lbrack", "rbrack", "lsbrac", "rsbrac",
                "quo", "not", "write", "writeln", "read"};


    for(int i=0; i<46; i++)
    {
        resWords[i] = res[i];
    }
}

/********************************************************************
**** FUNCTION    Lexical
*********************************************************************
****DESCRIPTION: overloaded constructor.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
Lexical::Lexical(char *fileName)
{
    Token = unknownt;
    ch = ' ';
    LineNo = 0;

    inFile.open(fileName);

    if(!inFile)
    {
        cout << "Entered file does not exist." << endl;
        exit(99);
    }


    string res[46] = {"class", "public", "static", "void", "main", "String", "extends", "return",
                "int","boolean", "if", "else", "while", "System.out.println", "length", 
               "true","false", "this", "new", "final", "num", "id", "relop", "addop","mulop",
                "signop","assignop", "symbol", "eof", "other", "unknown", "literal", "semi",
                "comma", "period", "lparen", "rparen", "lbrack", "rbrack", "lsbrac", "rsbrac",
                "quo", "not", "write", "writeln", "read"};

    for(int i=0; i<46; i++)
    {
        resWords[i] = res[i];
    }

}

/********************************************************************
**** FUNCTION    GetNextToken
*********************************************************************
****DESCRIPTION:Get the next token.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::GetNextToken()
{ 
  Value = 0;
  ValueR = "";

  while(isspace(ch))
  { 
    if(inFile.eof()){break;} 
    GetNextCh();
  }
  if(!inFile.eof())
  {
    Lexeme = "";
    ProcessToken();
  }
  else
  {
    inFile.close();
    Token = eoft;
  }
    
}

/********************************************************************
**** FUNCTION    GetNextCh
*********************************************************************
****DESCRIPTION:Get the character from the stream.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::GetNextCh()
{
    inFile.get(ch);
    if(ch == '\n'){LineNo++;}
}

/********************************************************************
**** FUNCTION    ProcessToken
*********************************************************************
****DESCRIPTION:Process tokens.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::ProcessToken()
{
  Lexeme.push_back(ch); 
  GetNextCh();
   if(isalpha(Lexeme[0])){ ProcessWordToken();}
   else if (isdigit(Lexeme[0])){ ProcessNumToken();}
   else if (Lexeme[0] == '/'){ ProcessComment();}
   else if (Lexeme[0] == '"'){ ProcessLiteral();}
   else if (Lexeme[0] == '=' ||Lexeme[0] == '!'|| Lexeme[0] == '<'||Lexeme[0] == '>')
   { 
       if(ch == '='){ProcessDoubleToken();}
       else{ ProcessSingleToken();}     
   }
   else if (Lexeme[0] == '+' ||Lexeme[0] == '-'){ ProcessAddop();}
   else if(Lexeme[0] == '|')
   {
       if(ch == '|') { ProcessDoubleAddop();}
       else{ cout << "ERROR: Invalid Token." << endl;}
   }
   else if (Lexeme[0] == '*' ||Lexeme[0] == '/'){ ProcessMulop();}
   else if(Lexeme[0] == '&')
   {
       if(ch == '&') { ProcessDoubleMulop();}
       else{ cout << "ERROR: Invalid Token." << endl;}
   }
   else if (Lexeme[0] == '(' ||Lexeme[0] == ')'||Lexeme[0] == '{' ||Lexeme[0] == '}' ||
            Lexeme[0] == ',' ||Lexeme[0] == ';' || Lexeme[0] == '.' || Lexeme[0] == '[' ||Lexeme[0] == ']')
            { ProcessOther();}
   else{ cout << "ERROR: Invalid Token." << endl;}
}

/********************************************************************
**** FUNCTION    ProcessOther
*********************************************************************
****DESCRIPTION:Process other tokens.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/ 
void Lexical::ProcessOther()
{
    if(Lexeme[0] == '(') { Token = lparent;}
    else if(Lexeme[0] == ')') { Token = rparent;}
    else if(Lexeme[0] == '{') { Token = lbrackt;}
    else if(Lexeme[0] == '}') { Token = rbrackt;}
    else if(Lexeme[0] == ',') { Token = commat;}
    else if(Lexeme[0] == ';') { Token = semit;}
    else if(Lexeme[0] == '.') { Token = periodt;}
    else if(Lexeme[0] == '[') { Token = lsbract;}
    else { Token = rsbract;}
}

/********************************************************************
**** FUNCTION    ProcessWordToken
*********************************************************************
****DESCRIPTION:Process alphabetic tokens.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::ProcessWordToken()
{
    Symbol sym;
    int i=1;
    int cc = 0;
    bool flag = true;

    while (isalnum(ch))
    {
        Lexeme.push_back(ch);
        i++;
        GetNextCh();
    }
    
    if(i <= 31)
    {
        for(sym=classt; sym <= readt; sym = static_cast<Symbol>(sym+1))
        {
            if(resWords[sym] == Lexeme)
            {

                Token = sym;
                flag = false;
                return;
            }
            cc++;
        }

        if(flag) { Token = idt;}
    }
    else 
    {
        cout << "ERROR: Token too long." << endl;
    }
} 

/********************************************************************
**** FUNCTION    ProcessNumToken
*********************************************************************
****DESCRIPTION:Process numeric tokens.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::ProcessNumToken()
{
    iflag = true;
    ValueR = "";
    Value = 0;

    while (isdigit(ch) || ch == '.')
    { 
        if(ch == '.')
        {
            iflag = false;
        }
        Lexeme.push_back(ch);
        GetNextCh();
    }
    
    if(!iflag)
    {
        ValueR = Lexeme;
        Token = numt;
    }
    else
    {
        Value = stoi(Lexeme);
        Token = numt;
    }
    
}

/********************************************************************
**** FUNCTION    ProcessComment
*********************************************************************
****DESCRIPTION:Process single/double comments.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::ProcessComment()
{
    char prev = ' ';
    char next = ' ';

    if(ch == '/')
    {
        while(ch != '\n')
        {   
            GetNextCh();
        }
        Token = othert;
    }
    else if(ch != '/' && ch !='*')
    {
        ProcessSingleToken();
    }
    else if(ch == '*')
    {
        GetNextCh();
        prev = ch;
        GetNextCh();
        next = ch;
        ch = prev;
        while(1)
        {
            if(ch == '*' && next == '/'){GetNextCh();break;}
            else if(inFile.eof()){ cout <<"ERROR: Comments not ending." << endl; break;}
            prev = next;
            GetNextCh();
            next = ch;
            ch = prev;
        }
        Token = othert;
    }
    else
    {
        cout << "ERROR: In comments." << endl;
    }
    
}

/********************************************************************
**** FUNCTION    ProcessLiteral
*********************************************************************
****DESCRIPTION:Process literals.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::ProcessLiteral()
{
    Literal = "";
    
    while(ch != '"')
    {
        if(ch == '\n')
        {
            cout << "ERROR: Literal exceeds one line." << endl;
            Token = othert;
            break;
        }
        else{
            Literal.push_back(ch);
            Token = literalt;
        }
        GetNextCh();
    }
    if(ch == '"'){
        GetNextCh();
    }
}

/********************************************************************
**** FUNCTION    ProcessDoubleToken
*********************************************************************
****DESCRIPTION:Process double tokens.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::ProcessDoubleToken()
{
   Token = relopt; 
   Lexeme.push_back('=');
   GetNextCh();
   
}

/********************************************************************
**** FUNCTION    ProcessSingleToken
*********************************************************************
****DESCRIPTION:Process a single token.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::ProcessSingleToken()
{
    if(Lexeme[0] == '=')
    {
        Token = assignopt;
    }
    else if(Lexeme[0] == '/')
    {
        Token = mulopt;
    }
    else if(Lexeme[0] == '!')
    {
        Token = nott;
    }
    else{
        Token = relopt;
    }
}

/********************************************************************
**** FUNCTION    ProcessAddop
*********************************************************************
****DESCRIPTION:Process add operator single/double.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::ProcessAddop()
{
    Token = addopt;
}

/********************************************************************
**** FUNCTION    ProcessMulop
*********************************************************************
****DESCRIPTION:Process mul operator single/double.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::ProcessMulop()
{
    Token = mulopt;
}

/********************************************************************
**** FUNCTION   ProcessDoubleAddop
*********************************************************************
****DESCRIPTION:Process double add operators.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::ProcessDoubleAddop()
{
    Token = addopt;
    Lexeme.push_back(ch);
    GetNextCh();
    
}

/********************************************************************
**** FUNCTION    ProcessDoubleMulop
*********************************************************************
****DESCRIPTION:Process double mul operators.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::ProcessDoubleMulop()
{
    Token = mulopt;
    Lexeme.push_back(ch);
    GetNextCh();
}

/********************************************************************
**** FUNCTION    DisplayToken
*********************************************************************
****DESCRIPTION:Display the current token
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Lexical::DisplayToken()
{
    if(Token == idt){
        cout << Lexeme << " is an identifier." << endl;
    }
    else if(Token == classt || Token ==  finalt){
        cout << Lexeme << " is a reserved word." << endl;
    }   
    else if(Token == numt){
        if(Value == 0)
        {
            if(ValueR.empty()){}
            else{
                cout << ValueR << " is a number." << endl;  
            }         
        }
        else
        {
            cout << Value << " is a number." << endl;
        }   
    }
    else if(Token == relopt){
        cout << Lexeme << " is a relational operator." << endl;
    }
    else if(Token == addopt){
        cout << Lexeme << " is an addop." << endl;
    }
    else if(Token == mulopt){
        cout << Lexeme << " is a mulop." << endl;
    }
    else if(Token == assignopt){
        cout << Lexeme << " is an assignop." << endl;
    }
    else if(Token == literalt){
        cout << Literal << " is a literal." << endl;
    }
    else if(Token == lparent){
        cout << Lexeme << " is a left parentheses." << endl;
    }
    else if(Token == rparent){
        cout << Lexeme << " is a right parentheses." << endl;
    }
    else if(Token == lbrackt){
        cout << Lexeme << " is a left bracket." << endl;
    }
    else if(Token == rbrackt){
        cout << Lexeme << " is a right bracket." << endl;
    }
    else if(Token == commat){
        cout << Literal << " is a comma." << endl;
    }
    else if(Token == semit){
        cout << Lexeme << " is a semicolon." << endl;
    }
    else if(Token == periodt){
        cout << Lexeme << " is a period." << endl;
    }
    else if(Token == lsbract){
        cout << Literal << " is a left square bracket." << endl;
    }
    else if(Token == rsbract){
        cout << Literal << " is a right square bracket." << endl;
    }
}

/********************************************************************
**** FUNCTION     ~Lexical
*********************************************************************
****DESCRIPTION: Destructor.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
Lexical::~Lexical()
{}