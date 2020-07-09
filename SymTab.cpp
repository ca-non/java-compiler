#include "SymTab.h"
#include "Lexical.h"
#include <iostream>
#include <string>
using namespace std;

/********************************************************************
**** FUNCTION    SymTab
*********************************************************************
****DESCRIPTION: constructor.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
SymTab::SymTab()
{
    InitTable();
}

/********************************************************************
**** FUNCTION    isEmpty
*********************************************************************
****DESCRIPTION:Check if the Symbol table is empty.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :bool
********************************************************************/
bool SymTab::isEmpty() 
{
    for(int i=0; i<TableSize; i++)
    {
        if(SymTable[i] != nullptr) {return false;}            
    }
    return true;
}


/********************************************************************
**** FUNCTION    hash
*********************************************************************
****DESCRIPTION:Passed a lexeme and return the location for that lexeme.
****INPUT ARGS :string
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :int
********************************************************************/
int SymTab::hash(string s)
{
    int h = 0, i = 0;

    while(s[i] != '\0')
    {
        h +=int(s[i]);
        i++;
    }

    return h % TableSize;
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
void SymTab::insert(string lex, Lexical::Symbol token, int depth)
{
    int hashVal = hash(lex);

    // create an actual table node
    EntryPtr newNode = new TableEntry;
    newNode->Token = token;
    newNode->Lexeme = lex;
    newNode->depth = depth;
    newNode->next = nullptr;

    if(!SymTable[hashVal])
    {
        SymTable[hashVal] = newNode;
    }
    else
    {
        EntryPtr temp = SymTable[hashVal];
        SymTable[hashVal] = newNode;
        newNode->next = temp;
        temp = nullptr;
    }

    return;
    
}


void SymTab::insertLiteral(string lex, string literal, Lexical::Symbol token, int depth)
{
    int hashVal = hash(lex);

    // create an actual table node
    EntryPtr newNode = new TableEntry;
    newNode->Token = token;
    newNode->Lexeme = lex;
    newNode->literal = literal;
    newNode->depth = depth;
    newNode->next = nullptr;

    if(!SymTable[hashVal])
    {
        SymTable[hashVal] = newNode;
    }
    else
    {
        EntryPtr temp = SymTable[hashVal];
        SymTable[hashVal] = newNode;
        newNode->next = temp;
        temp = nullptr;
    }

    return;
    
}

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
void SymTab::writeTable(int depth)
{
    cout << "Depth: " << depth << endl;
    for(int i=0; i<TableSize; i++)
    {
        if(!SymTable[i]) continue;
        else
        {
            EntryPtr temp = SymTable[i];

            while(temp!=nullptr)
            {
                // specified depth
                if(temp->depth == depth)
                {
                    // display the Lexeme
                    if(temp->TypeOfEntry == EntryType::constEntry){
                        cout << temp->Lexeme << "  " << "<constEntry>" << endl;
                    }
                    else if(temp->TypeOfEntry == EntryType::varEntry){
                        cout << temp->Lexeme << "  " << "<varEntry>" << endl;
                    }
                    else if(temp->TypeOfEntry == EntryType::functionEntry){
                        cout << temp->Lexeme << "  " << "<functionEntry>" << endl;
                    }
                    else {
                        cout << temp->Lexeme << "  " << "<classEntry>" << endl;
                    }

                }
                temp = temp->next;
            }
        }
    }
    cout << endl;
}

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
void SymTab::deleteDepth(int depth)
{
    for(int i=0; i<TableSize; i++)
    {
        if(!SymTable[i]) continue;
        else
        {
            EntryPtr temp = SymTable[i];
            EntryPtr prev = nullptr;

            while(temp!=nullptr)
            {
                // specified depth
                if(temp->depth == depth)
                {
                    // delete the record and adjust the chain
                    if(prev == nullptr)
                    {
                        SymTable[i] = temp->next;
                        delete temp;
                    }
                    else
                    {
                        prev->next= temp->next;   
                        delete temp;                  
                    } 
                }
                prev = temp;
                temp = temp->next;
            }

            temp = nullptr, prev = nullptr;
        }
    }
}

/********************************************************************
**** FUNCTION    InitTable
*********************************************************************
****DESCRIPTION:Initialise the symbol table to null.
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void SymTab::InitTable()
{
    for(int i=0; i<TableSize; i++)
    {
        SymTable[i] = nullptr;
    }
}

/********************************************************************
**** FUNCTION    ~SymTab
*********************************************************************
****DESCRIPTION:Destructor
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
SymTab::~SymTab()
{}


