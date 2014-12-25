#ifndef PARSEREXCEPTION
#define PARSEREXCEPTION
#include "myexception.hpp"
#include <string>

class ParserException : public MyException {
public:
	ParserException (std::string error):
	MyException(error)
	{
	}
};


#endif