#include "Lexical.h"
#include <iostream> 
#include <string>
using namespace std;

#ifndef SYMTAB_H
#define SYMTAB_H

class SymTab{
private:
    static const int TableSize = 211; //prime number
    // hash function
    int hash(string);

    bool isEmpty();

public:
    enum VarType {voidType, boolType, intType};

    enum EntryType {constEntry, varEntry, functionEntry, classEntry};

    struct MethodName;

    typedef MethodName * MethodPtr;

    struct MethodName {
        string name;
        MethodPtr next;
    };

    struct VarName;

    typedef VarName * VarPtr;

    struct VarName {
        string name;
        VarPtr next;
    };

    struct ParamNode;

    typedef ParamNode * ParamPtr;

    struct ParamNode {
        VarType typeOfParameter;
        ParamPtr Next;
    };
   
    struct TableEntry;

    typedef TableEntry * EntryPtr; //pointer to actual table entry

    struct TableEntry {
        Lexical::Symbol Token;
        string Lexeme;
        string literal;
        int depth;
        EntryType TypeOfEntry; // tag field for the union
        union {
            struct {
                VarType TypeOfVariable;
                int Offset;
                int size;
            } var;
            struct {
                VarType TypeOfConstant; //int or  bool
                int Offset;
                union {
                    int Value;
                    float ValueR;
                };
            } constant;
            struct { //this is the entry for a function
                int SizeOfLocal;
                int NumberOfParameters;
                int SizeParam;
                VarType ReturnType;
                ParamPtr ParamList; //linked list of paramter types
            } function;
            struct {  // class entry
                int SizeofLocal;
                MethodPtr MethodList;
                VarPtr VarList;
            } Class;
    }; //end of union
    EntryPtr next; //points to next list item
    };

    EntryPtr SymTable[TableSize]; //table of pointers}

    //functioprototypes for symbol table routines
/********************************************************************
**** FUNCTION    SymTab
*********************************************************************
****DESCRIPTION: constructor.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    SymTab();
    
/********************************************************************
**** FUNCTION    lookup
*********************************************************************
****DESCRIPTION:lookup uses the lexeme to find the entry and returns a 
****            pointer to that entry.
****INPUT ARGS :string lex
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :EntryPtr
********************************************************************/
    EntryPtr lookup(string lex)
    {
        int hashVal = hash(lex);
        if(!SymTable[hashVal])
        {
            return nullptr;
        } 
        else
        {

            EntryPtr temp = nullptr;
            temp = SymTable[hashVal];

            while(temp != nullptr)
            {
                if(temp->Lexeme == lex) {return temp;}

                temp = temp->next;
            }

            return temp;
        }
        
    }   

    EntryPtr lookup(string lex, int depth)
    {
        int hashVal = hash(lex);
        if(!SymTable[hashVal])
        {
            return nullptr;
        } 
        else
        {

            EntryPtr temp = nullptr;
            temp = SymTable[hashVal];

            while(temp != nullptr)
            {
                if(temp->Lexeme == lex && temp->depth == depth) {return temp;}

                temp = temp->next;
            }

            return temp;
        }
        
    }  
/********************************************************************
**** FUNCTION    insert
*********************************************************************
****DESCRIPTION:insert the lexeme, token and depth intoa record in the 
****            symbol table.
****INPUT ARGS :string, Lexical::Symbol, int
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void insert(string, Lexical::Symbol, int);
    void insertLiteral(string,string, Lexical::Symbol, int);

/********************************************************************
**** FUNCTION    writeTable
*********************************************************************
****DESCRIPTION:include a procedure that will write out all variables
****            (lexeme only) that are in the table at a specified 
****            depth
****INPUT ARGS :int
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void writeTable(int);

/********************************************************************
**** FUNCTION    deleteDepth
*********************************************************************
****DESCRIPTION:delete is passed the depth and deletes all records that are 
****            in the table at that depth
****INPUT ARGS :int
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void deleteDepth(int);

/********************************************************************
**** FUNCTION    InitTable
*********************************************************************
****DESCRIPTION:Initialise the symbol table to null.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    void InitTable();

/********************************************************************
**** FUNCTION    ~SymTab
*********************************************************************
****DESCRIPTION:Destructor
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
    ~SymTab();

};
 
#endif