COMPILER=g++
CFLAGS=-std=c++11 -g -c -Wall -Wextra --pedantic
SOURCES=header.cpp myexception.cpp noticeexception.cpp parserexception.cpp lexerexception.cpp dataexception.cpp alphabet.cpp token.cpp lexer.cpp data.cpp node.cpp parser.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=prog

all: $(SOURCES) $(HEADERS) $(EXECUTABLE) 

$(EXECUTABLE) :  $(OBJECTS)
	$(COMPILER) $(OBJECTS) -o $@



.cpp.o:
	$(COMPILER) $(CFLAGS) $< -o $@






