#ifndef TYPEEXCEPTION
#define TYPEEXCEPTION

#include "myexception.hpp"
#include <string>

class TypeException : public MyException {
public:
	TypeException (std::string error = "", Position position = {0, 0});
};

#endif