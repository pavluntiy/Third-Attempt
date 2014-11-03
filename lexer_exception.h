#include "my_exception.h"
#include <string>

class LexerException : public MyException {
public:
	LexerException (std::string error, Type type = Type::DEFAULT):
	MyException(error, type)
	{
	}
};