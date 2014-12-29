#include "parserexception.hpp"
	ParserException::ParserException (std::string error, Position position):
	MyException(error, position)
	{
	}
