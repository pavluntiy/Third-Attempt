#ifndef PARSEREXCEPTION
#define PARSEREXCEPTION
#include "my_exception.h"
#include <string>

class ParserException : public MyException {
public:
	ParserException (std::string error, Type type = Type::DEFAULT):
	MyException(error, type)
	{
	}
};


#endif