#ifndef PARSEREXCEPTION
#define PARSEREXCEPTION
#include "myexception.hpp"
#include <string>

class ParserException : public MyException {
public:
	ParserException (string error = "", Position position = {0, 0});
};


#endif