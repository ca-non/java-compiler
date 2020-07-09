#include "Parser.h"    
#include "Lexical.h"
#include "Error.h"
#include "SymTab.h"
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
using namespace std;

/********************************************************************
**** FUNCTION     Parser
*********************************************************************
****DESCRIPTION: constructor. Initiate the Parser
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
Parser::Parser(char *fileName)
{
    //initializations
    MyLex.Initiate(fileName);
    string outF="";
    int i = 0;
    FFlag = 0;
    outFileName = "";

    depth = 1;
    size = 0;
    localVarSize = 0;
    offSet = 0;
	parameterNum = 0;
    literalNo = 0;
    returnFlag = false;
    paraSizeT = 0;

    while(fileName[i] != '.'){
        outFileName.push_back(fileName[i]);
        i++;
    }
    outF = outFileName;
    outFileName.append(".tac");
    asmFileName = outF;
    outFile.open(outFileName);
    //writesAsmTop(outF);

    MyLex.GetNextToken();
    while(MyLex.Token == Lexical::Symbol::othert){
        
        MyLex.GetNextToken();
    }

    Prog();
     
    if(MyLex.Token == Lexical::eoft)
    {
       cout << "Successfull compilation" << endl;
    }

    
}

void Parser::writesAsmTop()
{
    int i = 0;
    string lit = ""; 
    string fileN = asmFileName + ".asm";
    
    std::ifstream ifs(fileN);
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                        (std::istreambuf_iterator<char>()    ) );
    ifs.close();

    std::ofstream asmFile(fileN);


    asmFile<<"                  .model small\n";
    asmFile<<"                  .586\n";
    asmFile<<"                  .stack 100h\n";
    asmFile<<"                  .data\n";
    // Insert Literals
    SymTab::EntryPtr tempPtr = nullptr;
    lit = "S" + std::to_string(i);
    tempPtr = S1.lookup(lit);

    while(tempPtr){
        asmFile<< lit <<"       DB      \""<<tempPtr->literal<<"\",\"$\"\n";

        i++;
        lit = "S" + std::to_string(i);
        tempPtr = nullptr;
        tempPtr = S1.lookup(lit);
    }
    
    asmFile<<"                  .code\n";
    asmFile<<"                  include io.asm\n";
    asmFile<<"start             PROC\n";
    asmFile<<"                  mov ax, @data\n";
    asmFile<<"                  mov ds, ax\n";
    asmFile<<"                  call main\n";
    asmFile<<"                  mov ah, 4ch\n";
    asmFile<<"                  mov al,0\n";
    asmFile<<"                  int 21h\n";
    asmFile<<"start             ENDP\n\n";
    asmFile << content;
    asmFile<<"\n";


}


void Parser::writesAsmMethods()
{
    string fileN = asmFileName + ".asm";
    string tempLine = "";
    string tacFileName = asmFileName + ".tac";
    ifstream tacFile;
    int iter_1;
    string procName = "";
    string temp="";
    string temp2="";
    string temp3="";
    int localVarSize = 0;
    int paraSize = 0;

    asmFile.open(fileN);
    
    tacFile.open(tacFileName);

    while(!tacFile.eof()){
        getline(tacFile,tempLine);

        if(tempLine[0] == '\n'){}
        else if(tempLine[0] == 'P'){ //PROC
            iter_1 = 0;
            while(!isblank(tempLine[iter_1])){
                iter_1++;
            }
            while(isblank(tempLine[iter_1])){
                iter_1++;
            }
            while(isalnum(tempLine[iter_1])){
                procName.push_back(tempLine[iter_1]);
                iter_1++;
            }

            // Get size of localVarable
            SymTab::EntryPtr localPtr = S1.lookup(procName);
            localVarSize = localPtr->function.SizeOfLocal;
            paraSize = localPtr->function.NumberOfParameters;

            asmFile<<setw(18)<<left<<procName<<"PROC\n";
            asmFile<<"                  push bp\n";
            asmFile<<"                  mov bp, sp\n";
            asmFile<<"                  sub sp, "<< localVarSize <<"\n";


            localVarSize = 0;           
        }
        else if(tempLine[0] == 'E'){ //Endp
            SymTab::EntryPtr localPtr = S1.lookup(procName);
            localVarSize = localPtr->function.SizeOfLocal;
            paraSize = localPtr->function.SizeParam;

            asmFile<<"                  add sp, "<< localVarSize <<"\n";
            asmFile<<"                  pop bp\n";
            asmFile<<"                  ret "<< paraSize <<"\n";
            asmFile<<setw(18)<<left<<procName<<"endp\n\n";

            paraSize = 0;
            procName ="";
        }
        else if(tempLine[0] == 'w'){ // wrs, wri, wrln
            if(tempLine[2] == 's'){ //wrs
                iter_1 = 0;
                while(tempLine[iter_1] != '_'){
                    iter_1++;
                }
                iter_1++;
                while(isalnum(tempLine[iter_1])){
                    temp.push_back(tempLine[iter_1]);
                    iter_1++;
                }

                asmFile<<"                  mov dx, offset "<<temp<<"\n";
                asmFile<<"                  call writestr\n";      

                temp = "";                         
            }
            else if(tempLine[2] == 'i'){ //wri
                iter_1 = 0;
                while(tempLine[iter_1] != '_'){
                    iter_1++;
                }
                iter_1++;
                while(isalnum(tempLine[iter_1]) || tempLine[iter_1] == '-' || tempLine[iter_1] == '+'){
                    temp.push_back(tempLine[iter_1]);
                    iter_1++;
                }

                asmFile<<"                  mov dx, ["<<temp<<"]\n";
                asmFile<<"                  call writeint\n";      

                temp = "";   
            }
            else if(tempLine[2] == 'l'){
                asmFile<<"                  call writeln\n";                 
            }
        }
        else if(tempLine[0] == 'r'){ //rdi
            iter_1 = 0;
            while(tempLine[iter_1] != '_'){
                iter_1++;
            }
            iter_1++;
            while(isalnum(tempLine[iter_1]) || tempLine[iter_1] == '-' || tempLine[iter_1] == '+'){
                temp.push_back(tempLine[iter_1]);
                iter_1++;
            }

            asmFile<<"                  call readint\n"; 
            asmFile<<"                  mov ["<<temp<<"], bx\n";     

            temp = "";                 
        }
        else if(tempLine[0] == '_'){ //_AX, _bp = 
            if(tempLine[1] == 'A'){ //_AX
              iter_1 = 0;
              while(tempLine[iter_1] != '='){
                  iter_1++;
              }
              iter_1++;
              while(isspace(tempLine[iter_1])){
                iter_1++;  
              }
              if(tempLine[iter_1] == '0'){
                asmFile<<"                  mov ax,0\n";
              }
              else{
                  iter_1++;
                  while(isalnum(tempLine[iter_1]) || tempLine[iter_1] == '-' || tempLine[iter_1] == '+'){
                      temp.push_back(tempLine[iter_1]);
                      iter_1++;
                  }
                  asmFile<<"                  mov ax, ["<<temp<<"]\n";     
              }
               
            }
            else{ 
              iter_1 = 0;
              temp="";
              iter_1++; //escape "_"
              while(isalnum(tempLine[iter_1]) || tempLine[iter_1] == '-' || tempLine[iter_1] == '+'){
                  temp.push_back(tempLine[iter_1]);
                  iter_1++;///Temp1
              }
              //find "="
              while(tempLine[iter_1] != '='){
                  iter_1++;
              }  
              iter_1++; // escape "="  
              while(isspace(tempLine[iter_1])){
                  iter_1++;
              } 
              temp2 = "";        
              if(isdigit(tempLine[iter_1])){
                    while(isdigit(tempLine[iter_1])){
                        temp2.push_back(tempLine[iter_1]);
                        iter_1++;//Temp2
                    }

                    if(tempLine[iter_1] == '\0'){
                        asmFile<<"                  mov ax, "<<temp2<<"\n";   
                        asmFile<<"                  mov ["<<temp<<"], ax\n";                    
                    }
                    else{
                        temp3="";
                        while(tempLine[iter_1] != '+' && tempLine[iter_1] != '*'){
                            iter_1++;
                        }
                        if(tempLine[iter_1] == '+'){
                            iter_1++; // escape "+"
                            while(isspace(tempLine[iter_1])){
                                iter_1++;
                            }

                            if(isdigit(tempLine[iter_1])){
                                while(isdigit(tempLine[iter_1])){
                                    temp3.push_back(tempLine[iter_1]);
                                    iter_1++;//Temp3
                                }  

                                asmFile<<"                  mov ax, "<<temp2<<"\n";
                                asmFile<<"                  add ax, "<<temp3<<"\n";
                                asmFile<<"                  mov ["<<temp<<"], ax\n";                          
                            }
                            else{ //_bp
                                iter_1++; // escape "_"
                                while(isalnum(tempLine[iter_1]) || tempLine[iter_1] == '-' || tempLine[iter_1] == '+'){
                                    temp3.push_back(tempLine[iter_1]);
                                    iter_1++;///Temp1
                                } 

                                asmFile<<"                  mov ax, "<<temp2<<"\n";
                                asmFile<<"                  add ax, ["<<temp3<<"]\n";
                                asmFile<<"                  mov ["<<temp<<"], ax\n";  
                            }
                        }
                        else if(tempLine[iter_1] == '*'){
                            iter_1++; // escape "*"
                            while(isspace(tempLine[iter_1])){
                                iter_1++;
                            }

                            if(isdigit(tempLine[iter_1])){
                                while(isdigit(tempLine[iter_1])){
                                    temp3.push_back(tempLine[iter_1]);
                                    iter_1++;//Temp3
                                }  

                                asmFile<<"                  mov ax, "<<temp2<<"\n";
                                asmFile<<"                  mov bx, "<<temp3<<"\n";
                                asmFile<<"                  imul bx\n";
                                asmFile<<"                  mov ["<<temp<<"], ax\n";                          
                            }
                            else{ //_bp
                                iter_1++; // escape "_"
                                while(isalnum(tempLine[iter_1]) || tempLine[iter_1] == '-' || tempLine[iter_1] == '+'){
                                    temp3.push_back(tempLine[iter_1]);
                                    iter_1++;///Temp1
                                } 

                                asmFile<<"                  mov ax, "<<temp2<<"\n";
                                asmFile<<"                  mov bx, ["<<temp3<<"]\n";
                                asmFile<<"                  imul bx\n";
                                asmFile<<"                  mov ["<<temp<<"], ax\n";  
                            }
                        }
                    }
              }
              else if(tempLine[iter_1] == '_' && tempLine[iter_1+1] == 'A'){//_AX
                    asmFile<<"                  mov ["<<temp<<"], ax\n";
              }
              else{
                    iter_1++; // escape "_"
                    while(isalnum(tempLine[iter_1]) || tempLine[iter_1] == '-' || tempLine[iter_1] == '+'){
                        temp2.push_back(tempLine[iter_1]);
                        iter_1++;///Temp2
                    }  

                    if(tempLine[iter_1] == '\0'){
                        asmFile<<"                  mov ax, ["<<temp2<<"]\n";   
                        asmFile<<"                  mov ["<<temp<<"], ax\n";                    
                    }
                    else{
                                        
                        while(tempLine[iter_1] != '+' && tempLine[iter_1] != '*'){
                            iter_1++;
                        }
                        if(tempLine[iter_1] == '+'){
                            iter_1++; // escape "+"
                            while(isspace(tempLine[iter_1])){
                                iter_1++;
                            }

                            if(isdigit(tempLine[iter_1])){
                                while(isdigit(tempLine[iter_1])){
                                    temp3.push_back(tempLine[iter_1]);
                                    iter_1++;//Temp3
                                }  

                                asmFile<<"                  mov ax, ["<<temp2<<"]\n";
                                asmFile<<"                  add ax, "<<temp3<<"\n";
                                asmFile<<"                  mov ["<<temp<<"], ax\n";                          
                            }
                            else{ //_bp
                                iter_1++; // escape "_"
                                while(isalnum(tempLine[iter_1]) || tempLine[iter_1] == '-' || tempLine[iter_1] == '+'){
                                    temp3.push_back(tempLine[iter_1]);
                                    iter_1++;///Temp1
                                } 

                                asmFile<<"                  mov ax, ["<<temp2<<"]\n";
                                asmFile<<"                  add ax, ["<<temp3<<"]\n";
                                asmFile<<"                  mov ["<<temp<<"], ax\n";  
                            }
                        }
                        else if(tempLine[iter_1] == '*'){
                            iter_1++; // escape "*"
                            while(isspace(tempLine[iter_1])){
                                iter_1++;
                            }

                            if(isdigit(tempLine[iter_1])){
                                while(isdigit(tempLine[iter_1])){
                                    temp3.push_back(tempLine[iter_1]);
                                    iter_1++;//Temp3
                                }  

                                asmFile<<"                  mov ax, ["<<temp2<<"]\n";
                                asmFile<<"                  mov bx, "<<temp3<<"\n";
                                asmFile<<"                  imul bx\n";
                                asmFile<<"                  mov ["<<temp<<"], ax\n";                          
                            }
                            else{ //_bp
                                iter_1++; // escape "_"
                                while(isalnum(tempLine[iter_1]) || tempLine[iter_1] == '-' || tempLine[iter_1] == '+'){
                                    temp3.push_back(tempLine[iter_1]);
                                    iter_1++;///Temp1
                                } 

                                asmFile<<"                  mov ax, ["<<temp2<<"]\n";
                                asmFile<<"                  mov bx, ["<<temp3<<"]\n";
                                asmFile<<"                  imul bx\n";
                                asmFile<<"                  mov ["<<temp<<"], ax\n";  
                            }
                        }
                    }           
              }
              temp="";
            }
        }
        else if(tempLine[0] == 'c'){ //call
            iter_1 = 4;
            temp="";
            while(isspace(tempLine[iter_1])){
                iter_1++;
            }
            while(isalnum(tempLine[iter_1])){
                temp.push_back(tempLine[iter_1]);
                iter_1++;
            }
            asmFile<<"                  call "<<temp<<"\n";  
            temp="";          
        }
        else if(tempLine[0] == 'p' && tempLine[1] == 'u'){ //push
            iter_1 = 4;
            temp="";
            while(isspace(tempLine[iter_1])){
                iter_1++;
            }
            iter_1++;
            while(isalnum(tempLine[iter_1]) || tempLine[iter_1] == '-' || tempLine[iter_1] == '+'){
                temp.push_back(tempLine[iter_1]);
                iter_1++;
            }
            asmFile<<"                  mov ax, ["<<temp<<"]\n"; 
            asmFile<<"                  push ax\n"; 
            temp="";
        }

    }

    tacFile.close();
}

void Parser::writesAsmMain()
{
    ifstream tacFile;
    string tacFileName = asmFileName + ".tac";
    string tempLine="";
    string procName="";
    string temp="";
    int iter_1 = 0;
    
    tacFile.open(tacFileName);

    while(!tacFile.eof()){
        getline(tacFile,tempLine);   
        if(tempLine[0] == 'P'){ //PROC
            iter_1 = 0;
            while(!isblank(tempLine[iter_1])){
                iter_1++;
            }
            while(isblank(tempLine[iter_1])){
                iter_1++;
            }
            while(isalnum(tempLine[iter_1])){
                procName.push_back(tempLine[iter_1]);
                iter_1++;
            }
            
            if(procName == "main"){
                while(!tacFile.eof()){
                    getline(tacFile,tempLine);  
                    iter_1 = 0 ;
                    if(tempLine[0] == 'c'){
                        
                        iter_1 = 4;
                        while(isspace(tempLine[iter_1])){
                            iter_1++;
                        }
                        while(isalnum(tempLine[iter_1])){
                            temp.push_back(tempLine[iter_1]);
                            iter_1++;
                        }
                        asmFile<<"\nmain              PROC\n";
                        asmFile<<"                  call "<<temp<<"\n";
                        asmFile<<"                  ret\n";
                        asmFile<<"main              endp\n";
                        asmFile<<"END               start\n\n";
                    }
                    else{
                        asmFile<<"\nmain              PROC\n";
                        asmFile<<"                  ret\n";
                        asmFile<<"main              endp\n";
                        asmFile<<"END               start\n\n";                       
                    }
                    break;
                }
            }
            procName="";
        }
    }

    tacFile.close();
    asmFile.close();
}

/********************************************************************
**** FUNCTION    checkForDup
*********************************************************************
****DESCRIPTION:Check for duplicate in the same depth
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::checkForDup()
{
    SymTab::EntryPtr temp = nullptr;
    temp =  S1.lookup(MyLex.Lexeme);

    if(temp != nullptr && depth == temp->depth)
    {
        cout << "Line:"<<MyLex.LineNo+1<< " error: redeclaration of " << MyLex.Lexeme << endl;
        temp = nullptr;
        exit(99);
    }
    temp = nullptr;
}

/********************************************************************
**** FUNCTION   Prog
*********************************************************************
****DESCRIPTION:Prog -> MoreClasses MainClass
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::Prog()
{
    MoreClasses();
    MainClass();
    //S1.writeTable(depth);
    //1.deleteDepth(depth);
}

/********************************************************************
**** FUNCTION     match
*********************************************************************
****DESCRIPTION:Checks if the current token match the desired value
****INPUT ARGS :Lexical::Symbol s
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::match(Lexical::Symbol s)
{
    int t1,t2;
    if(MyLex.Token == s)
    {
        MyLex.GetNextToken();
        while(MyLex.Token == Lexical::Symbol::othert){
            
            MyLex.GetNextToken();
        }
    }
    else
    {
        t1 = s;
        t2 = MyLex.Token;
        MyErr.detect(1, MyLex.LineNo, t1, t2);
        //halt
    }  
}

/********************************************************************
**** FUNCTION     MainClass
*********************************************************************
****DESCRIPTION:MainClass -> Finalt Classt Idt {
****                PublicT StaticT VoidT MainT ( StringT [ ] IdT ) 
****                    {SeqOfStatements}
****                }
****            }
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::MainClass()
{
    match(Lexical::Symbol::finalt);
    match(Lexical::Symbol::classt);
    S1.insert(MyLex.Lexeme, Lexical::Symbol::idt, depth);
    SymTab::EntryPtr temp = S1.lookup(MyLex.Lexeme);
    temp->TypeOfEntry = SymTab::classEntry;

    match(Lexical::Symbol::idt);
    depth++;
    match(Lexical::Symbol::lbrackt);
    match(Lexical::Symbol::publict);
    match(Lexical::Symbol::statict);
    match(Lexical::Symbol::voidt);
    checkForDup();
    S1.insert(MyLex.Lexeme, Lexical::Symbol::maint, depth);
    temp = S1.lookup(MyLex.Lexeme);
    temp->TypeOfEntry = SymTab::functionEntry;

    //emit-code
    emit("Proc  main\n");
    match(Lexical::Symbol::maint);
    match(Lexical::Symbol::lparent);
    depth++;
    match(Lexical::Symbol::Stringt);
    match(Lexical::Symbol::lsbract);
    match(Lexical::Symbol::rsbract);
    checkForDup();
    S1.insert(MyLex.Lexeme, Lexical::Symbol::idt, depth);
    temp = S1.lookup(MyLex.Lexeme);
    temp->TypeOfEntry = SymTab::varEntry;
    
    match(Lexical::Symbol::idt);
    match(Lexical::Symbol::rparent);
    match(Lexical::Symbol::lbrackt);
    SeqOfStatments();
    //emit-code
    emit("Endp  main\n");
    match(Lexical::Symbol::rbrackt);
    //S1.writeTable(depth);
    S1.deleteDepth(depth);
    depth--;
    //S1.writeTable(depth);
    S1.deleteDepth(depth);
    depth--;
    outFile.close();
    writesAsmMain();
    writesAsmTop();
    match(Lexical::Symbol::rbrackt);
    
}

/********************************************************************
**** FUNCTION     MoreClasses
*********************************************************************
****DESCRIPTION:MoreClasses -> ClassDecl MoreClasses | ε
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::MoreClasses()
{
    if(MyLex.Token == Lexical::Symbol::classt)
    {
        ClassDecl();
        //S1.writeTable(depth);
        MoreClasses();
    }
    else
    {
        //Nullable
    }
    
}

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
void Parser::ClassDecl()
{
    SymTab::MethodPtr tempM;
    SymTab::EntryPtr temp;
    if(MyLex.Token == Lexical::Symbol::classt)
    {
        int i;
        classlocalVarSize = 0;
        //check for classt-outer
        match(Lexical::Symbol::classt);

        // insert to the Symbol Table
        checkForDup();
        classNameGlo = MyLex.Lexeme;
        S1.insert(MyLex.Lexeme, Lexical::idt, depth);
        temp = S1.lookup(MyLex.Lexeme);
        temp->TypeOfEntry = SymTab::classEntry;
        match(Lexical::Symbol::idt);

        if(MyLex.Token == Lexical::Symbol::lbrackt)
        {
            match(Lexical::Symbol::lbrackt);
            depth++; // increment depth
            localVarSize = 0; 
            offSet = 0;
            varNames.clear(); //initialize string vector
            VarDecl();
            temp->Class.SizeofLocal = localVarSize;
            // add string vector to the class
            i=0;
            SymTab::VarPtr varNameList = nullptr;
                        
            while(!varNames.empty()){
                SymTab::VarPtr temp = new SymTab::VarName;
                temp->name = varNames.back();
                temp->next = nullptr;
                varNames.pop_back();

                if(!varNameList){
                    varNameList = temp;
                }
                else{
                    SymTab::VarPtr iter = varNameList;

                    while(iter->next){
                        iter = iter->next;
                    }
                    iter->next = temp;
                }
                i++;
            }
            methodNames.clear(); // initialize method vecor
            localVarSize = 0; 
            offSet = 0; 

            MethodDecl();
            // add method vector to the class
            i=0;
            // add to asm file
            outFile.close();
            writesAsmMethods();
            outFile.open(outFileName, std::ios_base::app);

            match(Lexical::Symbol::rbrackt);

            //S1.writeTable(depth);
            S1.deleteDepth(depth);
            depth--;
        }
        else if(MyLex.Token == Lexical::Symbol::extendst)
        {
            match(Lexical::Symbol::extendst);

            // insert to the Symbol Table
            checkForDup();
            S1.insert(MyLex.Lexeme, Lexical::idt, depth);
            SymTab::EntryPtr temp = S1.lookup(MyLex.Lexeme);
            temp->TypeOfEntry = SymTab::classEntry;
            match(Lexical::Symbol::idt);
            depth++;
            match(Lexical::Symbol::lbrackt);

            localVarSize = 0; 
            offSet = 0;
            varNames.clear(); //initialize string vector

            VarDecl();

            temp->Class.SizeofLocal = localVarSize;
            // add string vector to the class
            i=0;
            SymTab::VarPtr varNameList = nullptr;
            while(!varNames.empty()){
                SymTab::VarPtr temp = new SymTab::VarName;
                temp->name = varNames.back();
                temp->next = nullptr;
                varNames.pop_back();

                if(!varNameList){
                    varNameList = temp;
                }
                else{
                    SymTab::VarPtr iter = varNameList;

                    while(iter->next){
                        iter = iter->next;
                    }
                    iter->next = temp;
                }
                i++;
            }
            
            methodNames.clear(); // initialize method vecor
            localVarSize = 0; 
            offSet = 0;

            MethodDecl();

            // add method vector to the class
            i=0;

            // add to asm file
            writesAsmMethods();

            match(Lexical::Symbol::rbrackt);
            //S1.writeTable(depth);
            S1.deleteDepth(depth);
            depth--;
        }
        else
        {
            MyErr.detect(2, MyLex.LineNo, 1, MyLex.Token);
        }
    }
    else
    {
        
    }
    
}   

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
void Parser::VarDecl()
{

    if(MyLex.Token == Lexical::Symbol::intt || MyLex.Token == Lexical::Symbol::booleant
       || MyLex.Token == Lexical::Symbol::voidt)
    {
        Type();
        localVarSize += size;
        offSet += size;
        IdentifierList();
        match(Lexical::semit);
        VarDecl();
    }
    else if(MyLex.Token == Lexical::Symbol::finalt)
    {
        match(Lexical::Symbol::finalt);
        Type();
        localVarSize += size;
        offSet += size;
        checkForDup();
        S1.insert(MyLex.Lexeme, Lexical::idt, depth);
        varNames.push_back(MyLex.Lexeme);
        SymTab::EntryPtr temp = S1.lookup(MyLex.Lexeme);
        temp->TypeOfEntry = SymTab::constEntry;
        temp->constant.TypeOfConstant = varType;     
        temp->constant.Offset = offSet;

        match(Lexical::Symbol::idt);
        match(Lexical::Symbol::assignopt);
        if(varType == SymTab::intType){
            temp->constant.Value = stoi(MyLex.Lexeme);
        }
        match(Lexical::Symbol::numt);
        match(Lexical::Symbol::semit);
        VarDecl();
    }
    else
    {
        //Nullable
    }   
}

/********************************************************************
**** FUNCTION     IdentifierList
*********************************************************************
****DESCRIPTION:IdentifierList -> IdT commaT
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::IdentifierList()
{
    if(MyLex.Token == Lexical::Symbol::idt )
    {
         
        checkForDup();
        S1.insert(MyLex.Lexeme, Lexical::Symbol::idt, depth);
        varNames.push_back(MyLex.Lexeme);
        SymTab::EntryPtr temp = S1.lookup(MyLex.Lexeme);
        temp->TypeOfEntry = SymTab::varEntry;
        temp->var.TypeOfVariable = varType;
        temp->var.size = size;
        temp->var.Offset = offSet;        
        match(Lexical::Symbol::idt);
        IdentifierList();
    }
    else if(MyLex.Token == Lexical::Symbol::commat)
    {
        match(Lexical::Symbol::commat);
        checkForDup();
        localVarSize += size;
        offSet += size;
        S1.insert(MyLex.Lexeme, Lexical::Symbol::idt, depth);
        varNames.push_back(MyLex.Lexeme);
        SymTab::EntryPtr temp = S1.lookup(MyLex.Lexeme);
        temp->TypeOfEntry = SymTab::varEntry;
        temp->var.TypeOfVariable = varType;
        temp->var.size = size;
        temp->var.Offset = offSet;  

        match(Lexical::Symbol::idt);
        IdentifierList();
    }

}

/********************************************************************
**** FUNCTION     Type
*********************************************************************
****DESCRIPTION:Type -> IntT | FloatT | BooleanT | VoidT
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::Type()
{
    if(MyLex.Token == Lexical::Symbol::intt)
    {
        match(Lexical::Symbol::intt);     
        size = 2;
        varType = SymTab::intType;
    }
    else if(MyLex.Token == Lexical::Symbol::booleant)
    {
        match(Lexical::Symbol::booleant);
        size = 1;
        varType = SymTab::boolType;
    }
    else if(MyLex.Token == Lexical::Symbol::voidt)
    {
        match(Lexical::Symbol::voidt);
        size = 0;
        varType = SymTab::voidType;
    } 
    else
    {
        MyErr.detect(4, MyLex.LineNo, 1, MyLex.Token);
    }
    
}

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
void Parser::MethodDecl()
{
    SymTab::EntryPtr tempHolder = nullptr;
    SymTab::MethodPtr tempM;
    SymTab::EntryPtr tableTemp=nullptr;
    int i = 0;
    while(i < 6){
            paraList[i].Lexeme = "";
            paraList[i].offset = 0;
            i++;
    }
    if(MyLex.Token == Lexical::Symbol::publict)
    {   
        string methName = "";
        match(Lexical::Symbol::publict);
        Type();
        returnType = varType;
        checkForDup();
        S1.insert(MyLex.Lexeme, Lexical::idt, depth);
        SymTab::EntryPtr temp = S1.lookup(MyLex.Lexeme);
        temp->TypeOfEntry = SymTab::functionEntry;
        funcName = MyLex.Lexeme;
        methodNames.push_back(MyLex.Lexeme);
            
        tableTemp = S1.lookup(classNameGlo, 1);

        SymTab::MethodPtr methodList = tableTemp->Class.MethodList;//
        //  while(!methodNames.empty()){
        tempM = new SymTab::MethodName;
        tempM->name = MyLex.Lexeme;
        tempM->next = nullptr;
        //methodNames.pop_back();
        if(!methodList){
            methodList = tempM;
        }
        else{
            SymTab::MethodPtr iter = methodList;

            while(iter->next){
                iter = iter->next;
            }
            iter->next = tempM;
        }
        // }
        tableTemp->Class.MethodList = methodList;

        methName = MyLex.Lexeme;
        
        //emit-code
        emit("Proc   " + methName + "\n");

        match(Lexical::Symbol::idt);
        depth++;
        match(Lexical::Symbol::lparent);
        parameterNum = 0;
        localVarSize = 0;
        temp->function.ParamList = nullptr;
        FormalList();
        temp->function.NumberOfParameters = parameterNum;// add para number
        temp->function.ReturnType = returnType;
        match(Lexical::Symbol::rparent);
        match(Lexical::Symbol::lbrackt);

        offSet = -2;
        VarDecl();
        if(parameterList[0]== ""){
            offSet+=size;
        }
        else{
            offSet+=size;
        }    

        SeqOfStatments();
        match(Lexical::Symbol::returnt);
        // modify return
        returnFlag = true;
        tempHolder = temp;

        Expr(tempHolder);
        returnFlag = false;


        match(Lexical::Symbol::semit);
        match(Lexical::Symbol::rbrackt);

        if(!tempHolder){
            temp->function.SizeOfLocal = localVarSize;
            temp->function.SizeParam = paraSizeT;
        }
        else{
            temp = tempHolder;
            temp->function.SizeOfLocal = localVarSize;
            temp->function.SizeParam = paraSizeT;
        }

        //emit-code
        emit("Endp   " + methName + "\n");

        //S1.writeTable(depth);
        S1.deleteDepth(depth);
        depth--;
        i=0;
        FFlag=+1;
        
        MethodDecl();    
    }
    else
    {
        //Nullable
    }
    
}

/********************************************************************
**** FUNCTION     FormalList
*********************************************************************
****DESCRIPTION:FormalList -> Type IdT FormalRest | ε
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::FormalList()
{
    int i=0;
    paraSizeT = 0;
    while(i < 6){
        paraList[i].Lexeme = "";
        paraList[i].offset = 0;
        i++;
    }
    if(MyLex.Token == Lexical::Symbol::intt || MyLex.Token == Lexical::Symbol::booleant
       || MyLex.Token == Lexical::Symbol::voidt)
    {
        //parameterNum++;
        Type();
	    paraSizeT += size;
        //offSet += size;
        checkForDup();
        S1.insert(MyLex.Lexeme, Lexical::idt, depth);
        
        SymTab::EntryPtr tempIdt = S1.lookup(MyLex.Lexeme);
        tempIdt->TypeOfEntry = SymTab::varEntry;
        tempIdt = nullptr;

        SymTab::EntryPtr temp = S1.lookup(funcName);

        SymTab::ParamPtr newNode = new SymTab::ParamNode;
        newNode->typeOfParameter = varType;
        newNode->Next = nullptr;

        temp->function.ParamList = newNode;
        //add to paraList

        paraList[0].Lexeme = MyLex.Lexeme;
        paraList[0].offset = 0;
        match(Lexical::Symbol::idt);
        temp = nullptr;
        FormalRest();
        
    }
    else
    {                
        //Nullable
    }  
}


/********************************************************************
**** FUNCTION     FormalRest
*********************************************************************
****DESCRIPTION:FormalRest -> , Type IdT FormalRest | ε
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::FormalRest()
{
    static int iter = 1;
    static int offS = 0;

    if(MyLex.Token == Lexical::Symbol::commat)
    {
        match(Lexical::Symbol::commat);
        Type();
        //parameterNum++;
        paraSizeT += size;
        //offSet += size;
        checkForDup();
        S1.insert(MyLex.Lexeme, Lexical::idt, depth);
        SymTab::EntryPtr tempIdt = S1.lookup(MyLex.Lexeme);
        tempIdt->TypeOfEntry = SymTab::varEntry;
        SymTab::EntryPtr temp = S1.lookup(funcName);

        SymTab::ParamPtr newNode = new SymTab::ParamNode;
        newNode->typeOfParameter = varType;
        newNode->Next = nullptr;

        SymTab::ParamPtr tempParaNode = temp->function.ParamList;

        while(tempParaNode->Next)
        {
            tempParaNode = tempParaNode->Next;
        }

        tempParaNode->Next = newNode;

        paraList[iter].Lexeme = MyLex.Lexeme;
        offS +=size;
        paraList[iter].offset = offS;
        iter++;

        match(Lexical::Symbol::idt);
        FormalRest();
    }
    else
    {
        //Nullable
    }  
}

/********************************************************************
**** FUNCTION     SeqOfStatments
*********************************************************************
****DESCRIPTION:SeqOfStatements -> Statement ; StatTail | ε
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::SeqOfStatments()
{
    if(MyLex.Token == Lexical::Symbol::idt||MyLex.Token == Lexical::Symbol::readt||MyLex.Token == Lexical::Symbol::writet||MyLex.Token == Lexical::Symbol::writelnt)
    {
        Statement();
        match(Lexical::Symbol::semit);

        StatTail();
    }
    else
    {
        //Nullable
    }

}

/********************************************************************
**** FUNCTION     StatTail
*********************************************************************
****DESCRIPTION:StatTail -> Statement ; StatTail | ε
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::StatTail()
{
    if(MyLex.Token == Lexical::Symbol::idt||MyLex.Token == Lexical::Symbol::readt||MyLex.Token == Lexical::Symbol::writet||MyLex.Token == Lexical::Symbol::writelnt)
    {
        Statement();
        match(Lexical::Symbol::semit);
        StatTail();
    }
    else
    {
        //Nullable
    }

}

/********************************************************************
**** FUNCTION     Statement
*********************************************************************
****DESCRIPTION:Statement -> AssignStat | IOStat
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::Statement()
{        

    if(MyLex.Token == Lexical::Symbol::idt)
    {
        AssignStat();
    }
    else
    {

        IOStat();
    }

}

//emit
void Parser::emit(string code)
{
    outFile << code;
}

// newTemp
SymTab::EntryPtr Parser::newTemp()
{
    int depthTAC;
    string temp = "_t";
    static int i=1, g=2;
    string intVal = std::to_string(i);
    temp.append(intVal);
    if(FFlag == 0){
        depthTAC = -(g+offSet);
    }
    else {
        depthTAC = -(g+offSet);
        depthTAC+=size;
    }
    localVarSize = localVarSize +2;
    S1.insert(temp, Lexical::Symbol::idt, depthTAC);
    outFile << "_bp" << depthTAC;
    i++;
    g = g + 2;

    return S1.lookup(temp);
}

/********************************************************************
**** FUNCTION     AssignStat
*********************************************************************
****DESCRIPTION:AssignStat -> idt = Expr | idt = MethodCall | MethodCall
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::AssignStat()
{
    //check for undeclared
    SymTab::EntryPtr idptr = nullptr, synPtr = nullptr;
    SymTab::EntryPtr tempPtr = nullptr;
    string code;
    int a = 2;
    int b = 4;
    int bTemp;
    int iter_1;

    idptr =  S1.lookup(MyLex.Lexeme);


    if(idptr != nullptr)
    {
        // identifier declared 
    }
    else{
        cout << "Line:"<<MyLex.LineNo+1<< " error: undeclared identifier " << MyLex.Lexeme << endl;
        exit(99);
    }
    if(MyLex.Token == Lexical::Symbol::idt)
    {
        match(Lexical::Symbol::idt);

        if(MyLex.Token == Lexical::Symbol::assignopt)
        {
            match(Lexical::Symbol::assignopt);

            
            tempPtr = S1.lookup(MyLex.Lexeme);

            if(tempPtr && tempPtr->TypeOfEntry == SymTab::classEntry){
                match(Lexical::Symbol::idt);
                MethodCall();
                code = "";
                code.append("_bp");
                if(idptr->TypeOfEntry == SymTab::EntryType::varEntry)
                {
                    iter_1 = 0;
                    while(paraList[iter_1].Lexeme != idptr->Lexeme && iter_1 <6){
                        iter_1++;
                    }
                    if(paraList[iter_1].Lexeme == idptr->Lexeme){
                        bTemp = (b + paraList[iter_1].offset); 
                        code.append("+"); 
                        code.append(std::to_string(bTemp));
                    }
                    else{
                        // If not in parameters
                        bTemp = -(a + idptr->var.Offset); 
                        code.append(std::to_string(bTemp));
                    }
                }
                else 
                {
                    code.append(std::to_string(synPtr->depth));
                }
                code.append(" =  ");
                code.append("_AX\n");
                emit(code);
            }
            else if(MyLex.Token == Lexical::Symbol::idt || MyLex.Token == Lexical::Symbol::numt || MyLex.Token == Lexical::Symbol::lparent)
            {
                Expr(synPtr);
                //write to file
                code = "";
                code.append("_bp");
                if(idptr->TypeOfEntry == SymTab::EntryType::varEntry)
                {
                    iter_1 = 0;
                    while(paraList[iter_1].Lexeme != idptr->Lexeme && iter_1 <6){
                        iter_1++;
                    }
                    if(paraList[iter_1].Lexeme == idptr->Lexeme){
                        bTemp = (b + paraList[iter_1].offset); 
                        code.append("+"); 
                        code.append(std::to_string(bTemp));
                    }
                    else{
                        // If not in parameters
                        bTemp = -(a + idptr->var.Offset); 
                        code.append(std::to_string(bTemp));
                    }
                }
                else 
                {
                    code.append(std::to_string(synPtr->depth));
                }
                code.append(" =  ");
                code.append("_bp");
                if(synPtr->TypeOfEntry == SymTab::EntryType::varEntry)
                {
                    iter_1 = 0;
                    while(paraList[iter_1].Lexeme != synPtr->Lexeme && iter_1 <6){
                        iter_1++;
                    }
                    if(paraList[iter_1].Lexeme == synPtr->Lexeme){
                        bTemp = (b + paraList[iter_1].offset); 
                        code.append("+"); 
                        code.append(std::to_string(bTemp));
                    }
                    else{
                        // If not in parameters
                        bTemp = -(a + synPtr->var.Offset); 
                        code.append(std::to_string(bTemp));
                    }
                }else{
                    code.append(std::to_string(synPtr->depth));
                }

                code.append("\n\n");
                emit(code);
            }
        }
        else if(MyLex.Token == Lexical::Symbol::periodt)
        {      
            MethodCall();
        }
    }

}

/********************************************************************
**** FUNCTION      Expr
*********************************************************************
****DESCRIPTION:Expr -> Relation | ε
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::Expr(SymTab::EntryPtr &synPtr)
{
    if(MyLex.Token == Lexical::Symbol::idt || MyLex.Token == Lexical::Symbol::numt
       || MyLex.Token == Lexical::Symbol::lparent || MyLex.Token == Lexical::Symbol::addopt
       || MyLex.Token == Lexical::Symbol::truet || MyLex.Token == Lexical::Symbol::falset
        || MyLex.Token == Lexical::Symbol::nott||MyLex.Token == Lexical::Symbol::semit)
       {                      
           Relation(synPtr);
       }
}

/********************************************************************
**** FUNCTION     IOStat
*********************************************************************
****DESCRIPTION:IOStat -> ε
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::IOStat()
{
    if(MyLex.Token == Lexical::Symbol::readt)
    {
        In_Stat();
    }
    else if(MyLex.Token == Lexical::Symbol::writet || MyLex.Token == Lexical::Symbol::writelnt)
    {
        Out_Stat();
    }
   
}


void Parser::In_Stat()
{
    if(MyLex.Token == Lexical::Symbol::readt)
    {
        match(Lexical::Symbol::readt);
        match(Lexical::Symbol::lparent);
        Id_List();
        match(Lexical::Symbol::rparent);
    }
}

void Parser::Out_Stat()
{
    if(MyLex.Token == Lexical::Symbol::writet)
    {
        match(Lexical::Symbol::writet);
        match(Lexical::Symbol::lparent);
        Write_List();
        match(Lexical::Symbol::rparent);
    }
    else
    {
        match(Lexical::Symbol::writelnt);
        match(Lexical::Symbol::lparent);
        Write_List();
        emit("wrln\n");
        match(Lexical::Symbol::rparent);
    }
    
}


void Parser::Id_List()
{
    string code="";
    int a = 2;
    int b = 4;
    int iter_1;
    int bTemp;

    if(MyLex.Token == Lexical::Symbol::idt)
    {
        SymTab::EntryPtr Tplace = S1.lookup(MyLex.Lexeme);
        if(Tplace->TypeOfEntry == SymTab::EntryType::varEntry)
        {
            code.append("_bp");

            iter_1 = 0;
            while(paraList[iter_1].Lexeme != Tplace->Lexeme && iter_1 <6){
                iter_1++;
            }
            if(paraList[iter_1].Lexeme == Tplace->Lexeme){
                bTemp = (b + paraList[iter_1].offset);
                code.append("+"); 
                code.append(std::to_string(bTemp));
            }
            else{
                // If not in parameters
                bTemp = -(a + Tplace->var.Offset); 
                code.append(std::to_string(bTemp));
            }
        }
        else 
        {
            code.append(Tplace->Lexeme);
        }
        code.append("\n");
        emit("rdi ");
        emit(code);
        match(Lexical::Symbol::idt);
        Id_List_Tail();
    } 
}


void Parser::Id_List_Tail()
{
    string code="";
    int a = 2;
    int b = 4;
    int iter_1;
    int bTemp;
    if(MyLex.Token == Lexical::Symbol::commat)
    {
        match(Lexical::Symbol::commat);
        
        if(MyLex.Token == Lexical::Symbol::idt)
        {
            SymTab::EntryPtr Tplace = S1.lookup(MyLex.Lexeme);
            if(Tplace->TypeOfEntry == SymTab::EntryType::varEntry)
            {
                code.append("_bp");

                iter_1 = 0;
                while(paraList[iter_1].Lexeme != Tplace->Lexeme && iter_1 <6){
                    iter_1++;
                }
                if(paraList[iter_1].Lexeme == Tplace->Lexeme){
                    bTemp = (b + paraList[iter_1].offset);
                    code.append("+"); 
                    code.append(std::to_string(bTemp));
                }
                else{
                    // If not in parameters
                    bTemp = -(a + Tplace->var.Offset); 
                    code.append(std::to_string(bTemp));
                }
            }
            else 
            {
                code.append(Tplace->Lexeme);
            }
            code.append("\n");
            match(Lexical::Symbol::idt);
            emit("rdi ");
            emit(code);
            match(Lexical::Symbol::idt);
            Id_List_Tail();
        }
        else
        {
            
        }
    }
    
}


void Parser::Write_List()
{
   if(MyLex.Token == Lexical::Symbol::idt || MyLex.Token == Lexical::Symbol::numt || MyLex.Token == Lexical::Symbol::literalt)
    {
        Write_Token();
        Write_List_Tail();
    }
}


void Parser::Write_List_Tail()
{
    if(MyLex.Token == Lexical::Symbol::commat)
    {
        match(Lexical::Symbol::commat);
        Write_Token();
        Write_List_Tail();
    }
    else
    {
        
    }
    
}


void Parser::Write_Token()
{
    int iter_1;
    int a =2;
    int b=4;
    string code="";
    int bTemp;
    string litTemp;

    if(MyLex.Token == Lexical::Symbol::idt)
    {
        SymTab::EntryPtr Tplace = S1.lookup(MyLex.Lexeme);
        if(Tplace->TypeOfEntry == SymTab::EntryType::varEntry)
        {
            code.append("_bp");

            iter_1 = 0;
            while(paraList[iter_1].Lexeme != Tplace->Lexeme && iter_1 <6){
                iter_1++;
            }
            if(iter_1>=6){
                iter_1=0;
            }
            if(paraList[iter_1].Lexeme == Tplace->Lexeme){
                bTemp = (b + paraList[iter_1].offset);
                code.append("+"); 
                code.append(std::to_string(bTemp));
            }
            else{
                // If not in parameters
                bTemp = -(a + Tplace->var.Offset); 
                code.append(std::to_string(bTemp));
            }
        }
        else 
        {
            code.append(Tplace->Lexeme);
        }
        code.append("\n");
        match(Lexical::Symbol::idt);
        emit("wri ");
        emit(code);
    }
    else if(MyLex.Token == Lexical::Symbol::numt)
    {
        emit("wri ");
        emit(MyLex.Lexeme);
        emit("\n");
        match(Lexical::Symbol::numt);
    }
    else if(MyLex.Token == Lexical::Symbol::literalt)
    {
        emit("wrs _");
        litTemp="";
        litTemp.append("S");
        litTemp.append(std::to_string(literalNo));
        emit(litTemp);
        S1.insertLiteral(litTemp,MyLex.Literal, Lexical::Symbol::idt, 1);
        literalNo++;
        emit("\n");
        match(Lexical::Symbol::literalt);
    }
}

/********************************************************************
**** FUNCTION     Relation
*********************************************************************
****DESCRIPTION:Relation -> SimpleExpr
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::Relation(SymTab::EntryPtr &Rplace)
{
    SimpleExpr(Rplace);
}

/********************************************************************
**** FUNCTION     SimpleExpr
*********************************************************************
****DESCRIPTION:SimpleExpr -> Term MoreTerm
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::SimpleExpr(SymTab::EntryPtr &Splace)
{
    SymTab::EntryPtr Tplace = new SymTab::TableEntry;
    Tplace->Lexeme=""; 
    SymTab::EntryPtr tmpptr=nullptr;
    string code = "";
    int bTemp;
    int a =2;
    int b=4;
    int iter_1;

    Term(Tplace);
    MoreTerm(Tplace);
    if(Tplace->Lexeme[0] == '_'){
        Splace = Tplace;
    }
    else{ 

        if(returnFlag){
            code.append("_AX");
        }
        else{
            tmpptr = newTemp();
        }

        code.append(" = ");

        if(Tplace->TypeOfEntry == SymTab::EntryType::varEntry)
        {
            code.append("_bp");

            iter_1 = 0;
            while(paraList[iter_1].Lexeme != Tplace->Lexeme && iter_1 <6){
                iter_1++;
            }
            if(iter_1>=6){
                iter_1=0;
            }
            if(paraList[iter_1].Lexeme == Tplace->Lexeme){
                bTemp = (b + paraList[iter_1].offset);
                code.append("+"); 
                code.append(std::to_string(bTemp));
            }
            else{
                // If not in parameters
                bTemp = -(a + Tplace->var.Offset); 
                code.append(std::to_string(bTemp));
            }
        }
        else if(returnFlag){
            code.append("0");
        }
        else //if-Lexeme=="" --> print = 0
        {
            code.append(Tplace->Lexeme);
        }
        
        code.append("\n"); 
        emit(code);
        Splace = tmpptr;
    }

}

/********************************************************************
**** FUNCTION     Term
*********************************************************************
****DESCRIPTION:Term -> Factor MoreFactor
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::Term(SymTab::EntryPtr &Tplace)
{
    SymTab::EntryPtr Fplace;
    
    Factor(Fplace);
    MoreFactor(Fplace); //inherited

    Tplace = Fplace;
}

/********************************************************************
**** FUNCTION     MoreTerm
*********************************************************************
****DESCRIPTION:MoreTerm -> Addop Term MoreTerm | ε
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None        
********************************************************************/
void Parser::MoreTerm(SymTab::EntryPtr &MTplace)
{
    string code = "";
    int a = 2;
    int bTemp;
    int iter_1;
    int b=4;

    if(MyLex.Token == Lexical::Symbol::addopt)
    {
        SymTab::EntryPtr Aplace;
        SymTab::EntryPtr tmpptr;
        code.append(" = ");

        
        if(MTplace->TypeOfEntry == SymTab::EntryType::varEntry||MTplace->TypeOfEntry == SymTab::EntryType::constEntry)
        {
            code.append("_bp");

            iter_1 = 0;
            while(paraList[iter_1].Lexeme != MTplace->Lexeme  && iter_1 <6){
                iter_1++;
            }
            if(paraList[iter_1].Lexeme == MTplace->Lexeme){
                bTemp = (b + paraList[iter_1].offset); 
                code.append(std::to_string(bTemp));
            }
            else{
                bTemp = -(a + MTplace->var.Offset); 
                code.append(std::to_string(bTemp));
            }
        }
        else 
        {
            code.append(MTplace->Lexeme);
        }
        code.append(" ");
        code.append(MyLex.Lexeme);
        code.append(" ");
        Addop();
        Term(Aplace);
        tmpptr = newTemp();

        
        if(Aplace->TypeOfEntry == SymTab::EntryType::varEntry)
        {
            code.append("_bp");
            iter_1 = 0;
            while(paraList[iter_1].Lexeme != Aplace->Lexeme && iter_1 <6){
                iter_1++;
            }
            if(paraList[iter_1].Lexeme == Aplace->Lexeme){
                bTemp = (b + paraList[iter_1].offset); 
                code.append(std::to_string(bTemp));
            }
            else{
                bTemp = -(a + Aplace->var.Offset); 
                code.append(std::to_string(bTemp));
                a = a + 2;
            }
        }
        else if(Aplace->Token == Lexical::Symbol::numt){
            code.append(Aplace->Lexeme);           
        }
        else 
        {
            code.append("_bp");
            code.append(std::to_string(Aplace->depth));
        }
        
        code.append("\n");
        MTplace = tmpptr;
        emit(code);
        MoreTerm(tmpptr);
    }
    
}

/********************************************************************
**** FUNCTION     Factor
*********************************************************************
****DESCRIPTION:Factor -> id | num | ( Expr ) | ! Factor | signOp Facor|
****             true | false
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::Factor(SymTab::EntryPtr &Fplace)
{
    if (MyLex.Token == Lexical::Symbol::idt)
    {
        //check for undeclared
        SymTab::EntryPtr temp = nullptr;
        temp =  S1.lookup(MyLex.Lexeme);

        if(temp != nullptr)
        {
            // identifier declared 
        }
        else{
            cout << "Line:"<<MyLex.LineNo+1<< " error: undeclared identifier " << MyLex.Lexeme << endl;
            exit(99);    
        }

        Fplace = S1.lookup(MyLex.Lexeme);
        match(Lexical::Symbol::idt);
    }
    else if (MyLex.Token == Lexical::Symbol::numt)
    {
        SymTab::EntryPtr temp = new SymTab::TableEntry;
        temp->Token = Lexical::Symbol::numt;
        temp->Lexeme = MyLex.Lexeme;
        Fplace = temp;
        match(Lexical::Symbol::numt);
    }
    else if (MyLex.Token == Lexical::Symbol::lparent)
    {
        match(Lexical::Symbol::lparent);
        Expr(Fplace);
        match(Lexical::Symbol::rparent);
    }
    else if (MyLex.Token == Lexical::Symbol::nott)
    {
        match(Lexical::Symbol::nott);
        Factor(Fplace);
    }
    else if (MyLex.Token == Lexical::Symbol::addopt)
    {
        string code ="";
        int bTemp;
        int a =2;
        SymTab::EntryPtr Aplace;
        SymTab::EntryPtr tmpptr;
        tmpptr = newTemp();
        code.append(" = ");
        code.append(MyLex.Lexeme);
        SignOp();
        Factor(Aplace);
        code.append("_bp");
        bTemp = -(a + Aplace->var.Offset);
        code.append(std::to_string(bTemp));
        code.append("\n");
        Fplace = tmpptr;
        emit(code); 
                  
    }
    else if (MyLex.Token == Lexical::Symbol::truet)
    {
        match(Lexical::Symbol::truet);
    }
    else if (MyLex.Token == Lexical::Symbol::falset)
    {
        match(Lexical::Symbol::falset);
    }
}

/********************************************************************
**** FUNCTION     MoreFactor
*********************************************************************
****DESCRIPTION:MoreFactor -> Mulop Factor MoreFactor |  ε
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::MoreFactor(SymTab::EntryPtr &Tplace)
{
    string code ="";
    int bTemp;
    int a = 2;
    int b =4;
    int iter_1;

    if(MyLex.Token == Lexical::Symbol::mulopt)
    {
        SymTab::EntryPtr Aplace;
        SymTab::EntryPtr tmpptr;
        code.append(" = ");
        code.append("_bp");

        if(Tplace->TypeOfEntry == SymTab::EntryType::varEntry)
        {
            iter_1 = 0;
            while(paraList[iter_1].Lexeme != Tplace->Lexeme && iter_1 <6){
                iter_1++;
            }
            if(paraList[iter_1].Lexeme == Tplace->Lexeme){
                bTemp = (b + paraList[iter_1].offset);
                code.append("+"); 
                code.append(std::to_string(bTemp));
            }
            else{
                bTemp = -(a + Tplace->var.Offset); 
                code.append(std::to_string(bTemp));
            }
        }
        else 
        {
            code.append(std::to_string(Tplace->depth));
        }

        code.append(" ");
        code.append(MyLex.Lexeme);
        code.append(" ");
        Mulop();
        Factor(Aplace);
        tmpptr = newTemp(); // start
        code.append("_bp");

        if(Aplace->TypeOfEntry == SymTab::EntryType::varEntry)
        {
            iter_1 = 0;
            while(paraList[iter_1].Lexeme != Aplace->Lexeme && iter_1 <6){
                iter_1++;
            }
            if(paraList[iter_1].Lexeme == Aplace->Lexeme){
                bTemp = (b + paraList[iter_1].offset); 
                code.append("+"); 
                code.append(std::to_string(bTemp));
            }
            else{
                bTemp = -(a + Aplace->var.Offset); 
                code.append(std::to_string(bTemp));
            }
        }
        else 
        {
            code.append(std::to_string(Aplace->depth));
        }

        code.append("\n");
        Tplace = tmpptr;
        emit(code);
        MoreFactor(tmpptr);
    }
    else 
    {
        
    }
}

/********************************************************************
**** FUNCTION     Addop
*********************************************************************
****DESCRIPTION:Addop -> + | - | ||
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::Addop()
{   
    if(MyLex.Token == Lexical::Symbol::addopt)
    {
        if(MyLex.Lexeme == "+")
        {
            match(Lexical::Symbol::addopt);
        }
        else if(MyLex.Lexeme == "-")
        {
            match(Lexical::Symbol::addopt);
        }
        if(MyLex.Lexeme == "||")
        {
            match(Lexical::Symbol::addopt);
        }
    }
}

/********************************************************************
**** FUNCTION     Mulop
*********************************************************************
****DESCRIPTION:Mulop -> * | / | &&
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::Mulop()
{
    if(MyLex.Token == Lexical::Symbol::mulopt)
    {
        if(MyLex.Lexeme == "*")
        {
            match(Lexical::Symbol::mulopt);
        }
        else if(MyLex.Lexeme == "/")
        {
            match(Lexical::Symbol::mulopt);
        }
        else if(MyLex.Lexeme == "&&")
        {
            match(Lexical::Symbol::mulopt);
        }
    }
}

/********************************************************************
**** FUNCTION     SignOp
*********************************************************************
****DESCRIPTION:SignOp -> -
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::SignOp()
{
    if(MyLex.Token == Lexical::Symbol::addopt)
    {
        match(Lexical::Symbol::addopt);
    }
}


/********************************************************************
**** FUNCTION   ParamsTail
*********************************************************************
****DESCRIPTION:ParamsTail -> , idt ParamsTail | , num ParamsTail | ε
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::ParamsTail()
{
    if(MyLex.Token == Lexical::Symbol::commat)
    {
        match(Lexical::Symbol::commat);

        if(MyLex.Token == Lexical::Symbol::idt)
        {
            //check for undeclared
            SymTab::EntryPtr temp = nullptr;
            temp =  S1.lookup(MyLex.Lexeme);
            if(temp != nullptr)
            {
                // identifier declared 
            }
            else{
                cout << "Line:"<<MyLex.LineNo+1<< " error: undeclared identifier " << MyLex.Lexeme << endl;
                exit(99);
            }

            parameterList[paraIter] = MyLex.Lexeme;
            paraIter++;

            match(Lexical::Symbol::idt);
            ParamsTail();
        }
        else if(MyLex.Token == Lexical::Symbol::numt)
        {
            parameterList[paraIter] = MyLex.Lexeme;
            paraIter++;

            match(Lexical::Symbol::numt);
            ParamsTail();
        }
    }
    else 
    {
    
    }
    
}

/********************************************************************
**** FUNCTION   Params
*********************************************************************
****DESCRIPTION:Params -> idt ParamsTail | num ParamsTail | ε
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::Params()
{
    int i=0;
    while(i < 6){
        parameterList[i] = "";
        i++;
    }
    paraIter = 0;

    if(MyLex.Token == Lexical::Symbol::idt)
    {
        //check for undeclared
        SymTab::EntryPtr temp = nullptr;
        temp =  S1.lookup(MyLex.Lexeme);
        if(temp != nullptr)
        {
            // identifier declared 
        }
        else{
            cout << "Line:"<<MyLex.LineNo+1<< " error: undeclared identifier " << MyLex.Lexeme << endl;
            exit(99);
        }

        parameterList[paraIter] = MyLex.Lexeme;
        paraIter++;

        match(Lexical::Symbol::idt);
        ParamsTail();
    }
    else if(MyLex.Token == Lexical::Symbol::numt)
    {
        parameterList[paraIter] = MyLex.Lexeme;
        paraIter++;

        match(Lexical::Symbol::numt);
        ParamsTail();
    }
    else
    {
        
    }
}

/********************************************************************
**** FUNCTION   ClassName
*********************************************************************
****DESCRIPTION:ClassName -> idt
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::ClassName()
{
    if(MyLex.Token == Lexical::Symbol::idt)
    {
        //check for undeclared
        SymTab::EntryPtr temp = nullptr;
        temp =  S1.lookup(MyLex.Lexeme);

        if(temp != nullptr)
        {
            // identifier declared 
        }
        else{
            cout << "Line:"<<MyLex.LineNo+1<< " error: undeclared identifier " << MyLex.Lexeme << endl;
            exit(99);
        }
        match(Lexical::Symbol::idt);
    }
    
}

/********************************************************************
**** FUNCTION   MethodCall
*********************************************************************
****DESCRIPTION:MethodCall -> ClassName.idt (Params)
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
void Parser::MethodCall()
{
    // className - matched in AssignStat()
    match(Lexical::Symbol::periodt); 

    //check for undeclared
    SymTab::EntryPtr temp = nullptr;
    SymTab::EntryPtr temp2 = nullptr;
    SymTab::MethodPtr mPtrTemp = nullptr;
    bool flag = false;

    temp =  S1.lookup(MyLex.Lexeme);

    //check for Class method name
    temp2  = S1.lookup(classNameGlo);
    
    if(temp2){
        mPtrTemp = temp2->Class.MethodList;

        while(mPtrTemp->next != nullptr){
            if(mPtrTemp->name == MyLex.Lexeme){
                flag = true;
                break;
            }
            mPtrTemp = mPtrTemp->next;
        }
        if(mPtrTemp->name == MyLex.Lexeme){
            flag = true;
        }
    }


    if(temp != nullptr || flag)
    {
        // identifier declared 
    }
    else{
        cout << "Line:"<<MyLex.LineNo+1<< " error: undeclared identifier " << MyLex.Lexeme << endl;
        exit(99);
    }     
    match(Lexical::Symbol::idt);
    match(Lexical::Symbol::lparent);
    Params();
    pushArgs();
    if(temp){
        emit("call " + temp->Lexeme + "\n");
    }
    else{
        emit("call " + mPtrTemp->name + "\n");        
    }
    match(Lexical::Symbol::rparent);
}


void Parser::pushArgs()
{
    int i = 0;
    int iter_1;
    int bTemp;
    int b=4;
    int a=2;
    
    while(parameterList[i] != ""){
        i++;
    }
    i--;
    while(i > -1){

        iter_1 = 0;
        while(paraList[iter_1].Lexeme != parameterList[i] && iter_1 <6){
            iter_1++;
        }
        if(iter_1 < 6){

        }
        else{
            iter_1=0;
        }
        if(paraList[iter_1].Lexeme == parameterList[i]){
            bTemp = (b + paraList[iter_1].offset);
            emit("push _bp+" + std::to_string(bTemp) + "\n");
        }
        else{
            // If not in parameters
            SymTab::EntryPtr temp = S1.lookup(parameterList[i]);
            bTemp = -(a + temp->var.Offset); 
            emit("push _bp" + std::to_string(bTemp) + "\n");          
        }         

        i--;
    }
}

/********************************************************************
**** FUNCTION     ~Parser
*********************************************************************
****DESCRIPTION: Destructor
****INPUT ARGS :None
****OUTPUT ARGS:None
****IN/OUT ARGS:None
****RETURN     :None
********************************************************************/
Parser::~Parser()
{}