#include "Lexical.h"
#include "Error.h"
#include "SymTab.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

#ifndef PARSER_H
#define PARSER_H

class Parser
{
public:
    Lexical MyLex;
    Error MyErr;
	SymTab S1;
	int depth;
	int size;
	SymTab::VarType varType;
    int localVarSize;
	int localMethodSize;
    int offSet;
	int parameterNum;
	int paraSizeT;
	SymTab::VarType returnType;
	string funcName;
	int classlocalVarSize;
	vector<string> varNames;
	vector<string> methodNames;
	ofstream outFile;
	const int SIZE = 81;

	struct paraMeters{
		string Lexeme;
		int offset;
	};
	struct paraMeters paraList[6];
	string parameterList[6];
	int paraIter;
	int FFlag;
	int literalNo;
	string classNameGlo;
	bool returnFlag;
	ofstream asmFile;
	string asmFileName;
	string outFileName;

	/********************************************************************
	**** FUNCTION     Parser
	*********************************************************************
	****DESCRIPTION: constructor. Initiate the Parser
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    Parser(char *);

	void MethodCall();
	void ClassName();
	void Params();
	void ParamsTail();
	void writesAsmTop();
	void writesAsmMethods();
	void writesAsmMain();



	/********************************************************************
	**** FUNCTION   Prog
	*********************************************************************
	****DESCRIPTION:Prog -> MoreClasses MainClass
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    void Prog();

	/********************************************************************
	**** FUNCTION     match
	*********************************************************************
	****DESCRIPTION: checks if the current token match the desired value
	****INPUT ARGS :Lexical::Symbol s
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    void match(Lexical::Symbol s);

	/********************************************************************
	**** FUNCTION     MainClass
	*********************************************************************
	****DESCRIPTION:MainClass -> Finalt Classt Idt {
	****                PublicT StaticT VoidT MainT ( StringT [ ] IdT ) 
	****                    SeqOfStatements
	****                }
	****            }
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    void MainClass();

	/********************************************************************
	**** FUNCTION     MoreClasses
	*********************************************************************
	****DESCRIPTION:MoreClasses -> ClassDecl MoreClasses | ε
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    void MoreClasses();

	/********************************************************************
	**** FUNCTION     ClassDecl
	*********************************************************************
	****DESCRIPTION:ClassDecl -> ClassT IdT { VarDecl MethodDecl } |
	****                         ClassT IdT ExtendsT IdT { VarDecl MethodDecl } |
	****                         ε
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    void ClassDecl();

	/********************************************************************
	**** FUNCTION     VarDecl
	*********************************************************************
	****DESCRIPTION:VarDecl -> Type IdentifierList ; VarDecl |
	****                       FinalT Type IdT = NumT ; VarDecl |
	****                       ε
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    void VarDecl();

	/********************************************************************
	**** FUNCTION     IdentifierList
	*********************************************************************
	****DESCRIPTION:IdentifierList -> IdT commaT
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    void IdentifierList();

	/********************************************************************
	**** FUNCTION     Type
	*********************************************************************
	****DESCRIPTION:Type -> IntT | FloatT | BooleanT | VoidT
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    void Type();

	/********************************************************************
	**** FUNCTION     MethodDecl
	*********************************************************************
	****DESCRIPTION: MethodDecl -> PublicT Type IdT ( FormalList ) {
	****                         VarDecl SeqOfStatements ReturnT Expr ;
	****                         } | 
	****                         ε 
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    void MethodDecl();

	/********************************************************************
	**** FUNCTION     FormalList
	*********************************************************************
	****DESCRIPTION:FormalList -> Type IdT FormalRest | ε
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    void FormalList();

	/********************************************************************
	**** FUNCTION     FormalRest
	*********************************************************************
	****DESCRIPTION:FormalRest -> , Type IdT FormalRest | ε
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    void FormalRest();

	/********************************************************************
	**** FUNCTION     SeqOfStatments
	*********************************************************************
	****DESCRIPTION:SeqOfStatements -> Statement ; StatTail | ε
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    void SeqOfStatments();

	/********************************************************************
	**** FUNCTION      Expr
	*********************************************************************
	****DESCRIPTION:  Expr function
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    void Expr(SymTab::EntryPtr &);

	/********************************************************************
	**** FUNCTION    checkForDup
	*********************************************************************
	****DESCRIPTION:Check for duplicate in the same depth
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void checkForDup();

	/********************************************************************
	**** FUNCTION     Statement
	*********************************************************************
	****DESCRIPTION:Statement -> AssignStat | IOStat
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void Statement();

	/********************************************************************
	**** FUNCTION     StatTail
	*********************************************************************
	****DESCRIPTION:StatTail -> Statement ; StatTail | ε
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void StatTail();

	/********************************************************************
	**** FUNCTION     IOStat
	*********************************************************************
	****DESCRIPTION:IOStat -> ε
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void IOStat();
	void In_Stat();
	void Out_Stat();
	void Id_List();
	void Id_List_Tail();
	void Write_List();
	void Write_List_Tail();
	void Write_Token();	

	/********************************************************************
	**** FUNCTION     Relation
	*********************************************************************
	****DESCRIPTION:Relation -> SimpleExpr
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void Relation(SymTab::EntryPtr &);

	/********************************************************************
	**** FUNCTION     SimpleExpr
	*********************************************************************
	****DESCRIPTION:SimpleExpr -> Term MoreTerm
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void SimpleExpr(SymTab::EntryPtr &);

	/********************************************************************
	**** FUNCTION     Term
	*********************************************************************
	****DESCRIPTION:Term -> Factor MoreFactor
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void Term(SymTab::EntryPtr &);

	/********************************************************************
	**** FUNCTION     MoreTerm
	*********************************************************************
	****DESCRIPTION:MoreTerm -> Addop Term MoreTerm | ε
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void MoreTerm(SymTab::EntryPtr &);

	/********************************************************************
	**** FUNCTION     Factor
	*********************************************************************
	****DESCRIPTION:Factor -> id | num | ( Expr ) | ! Factor | true | false
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void Factor(SymTab::EntryPtr &);

	/********************************************************************
	**** FUNCTION     MoreFactor
	*********************************************************************
	****DESCRIPTION:MoreFactor -> Mulop Factor MoreFactor |  ε
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void MoreFactor(SymTab::EntryPtr &);

	/********************************************************************
	**** FUNCTION     Addop
	*********************************************************************
	****DESCRIPTION:Addop -> + | - | ||
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void Addop();

	/********************************************************************
	**** FUNCTION     Mulop
	*********************************************************************
	****DESCRIPTION:Mulop -> * | / | &&
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void Mulop();

	/********************************************************************
	**** FUNCTION     SignOp
	*********************************************************************
	****DESCRIPTION:SignOp -> -
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void SignOp();

	/********************************************************************
	**** FUNCTION     AssignStat
	*********************************************************************
	****DESCRIPTION:AssignStat -> idt = Expr
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
	void AssignStat();

	void emit(string);

	SymTab::EntryPtr newTemp();
	void pushArgs();

	/********************************************************************
	**** FUNCTION     ~Parser
	*********************************************************************
	****DESCRIPTION: Destructor
	****INPUT ARGS :None
	****OUTPUT ARGS:None
	****IN/OUT ARGS:None
	****RETURN     :None
	********************************************************************/
    ~Parser();
};

#endif