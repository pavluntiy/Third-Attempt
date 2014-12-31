

#include "typeexception.hpp"

TypeException::TypeException (std::string error, Position position):
	MyException(error, position)
	{
	}


