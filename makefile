COMPILER=g++
CFLAGS=-std=c++11 -g -c -Wall -Wextra --pedantic
SOURCES=header.cpp alphabet.cpp\
	myexception.cpp\
	noticeexception.cpp parserexception.cpp lexerexception.cpp dataexception.cpp typeexception.cpp\
	\
	token.cpp\
	lexer.cpp\
	data.cpp\
	\
	basicvisitor.cpp\
	printvisitor.cpp deletevisitor.cpp scopeprintvisitor.cpp\
	\
	basicnode.cpp\
	operatorsnode.cpp functioncallnode.cpp valuenode.cpp programnode.cpp compoundnamenode.cpp \
	typenode.cpp vardeclarationnode.cpp signaturenode.cpp functiondefinitionnode.cpp ifnode.cpp whilenode.cpp fornode.cpp\
	structnode.cpp returnnode.cpp\
	\
	parser.cpp\
	\
	basicsymbol.cpp functionsymbol.cpp variablesymbol.cpp structuresymbol.cpp\
	type.cpp\
	\
	typecheckvisitor.cpp\
	basicscope.cpp globalscope.cpp functionscope.cpp structurescope.cpp localscope.cpp\
	main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=prog

all: $(SOURCES) $(HEADERS) $(EXECUTABLE) 

$(EXECUTABLE) :  $(OBJECTS)
	$(COMPILER) $(OBJECTS) -o $@

.cpp.o:
	$(COMPILER) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o






