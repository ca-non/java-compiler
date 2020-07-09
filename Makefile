a.out: main.o Lexical.o SymTab.o Parser.o Error.o
	g++ main.o Lexical.o SymTab.o Parser.o Error.o -o a.out

Lexical.o: Lexical.h Lexical.cpp 
	g++ -c Lexical.cpp

Parser.o: Parser.h Lexical.h Error.h SymTab.h Parser.cpp 
	g++ -c Parser.cpp

Error.o: Error.cpp Error.h
	g++ -c Error.cpp

SymTab.o: SymTab.h Lexical.h SymTab.cpp
	g++ -c SymTab.cpp

main.o: Parser.h main.cpp 
	g++ -c main.cpp

clean:
	rm a.out main.o Lexical.o Parser.o Error.o SymTab.o