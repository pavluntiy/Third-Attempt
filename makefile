COMPILER=g++
CFLAGS=-std=c++11 -g -c -Wall -Wextra --pedantic
SOURCES=header.cpp alphabet.cpp\
	myexception.cpp noticeexception.cpp parserexception.cpp lexerexception.cpp dataexception.cpp\
	token.cpp lexer.cpp data.cpp node.cpp\
	printvisitor.cpp\
	basicnode.cpp\
	expressionnode.cpp functioncallnode.cpp valuenode.cpp programnode.cpp compoundnamenode.cpp typenode.cpp\
	parser.cpp\
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






