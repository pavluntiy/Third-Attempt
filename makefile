COMPILER=g++
CFLAGS=-std=c++11 -g -c -Wall -Wextra --pedantic
HEADERS=header.h parser.h lexer.h data.h my_exception.h parser_exception.h lexer_exception.h data_exception.h node.h token.h alphabet.h 
SOURCES=main.cpp
GCH=$(HEADERS:.h=.gch)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=prog

all: $(SOURCES) $(HEADERS) $(EXECUTABLE) 

$(EXECUTABLE) :  $(OBJECTS)
	$(COMPILER) $(OBJECTS) -o $@

.h.gch:
	$(COMPILER) $(CFLAGS) $< -o $@

.cpp.o:
	$(COMPILER) $(CFLAGS) $< -o $@






