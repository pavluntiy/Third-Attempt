#ifndef DATAEXCEPTION
#define DATAEXCEPTION

#include "myexception.hpp"
#include <string>

class DataException : public MyException {
public:
	DataException (std::string error);
};

#endif