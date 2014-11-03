#ifndef DATA_EXCEPTION
#define DATA_EXCEPTION

#include "my_exception.h"
#include <string>

class DataException : public MyException {
public:
	DataException (std::string error, Type type = Type::DEFAULT):
	MyException(error, type)
	{
	}
};

#endif