

#include "lexerexception.hpp"

LexerException::LexerException (std::string error, Position position):
	MyException(error, position)
	{
	}


