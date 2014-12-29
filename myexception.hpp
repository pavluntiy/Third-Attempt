#ifndef MYEXCEPTION
#define MYEXCEPTION

#include <exception>
#include <string>

#include "header.hpp"

class MyException : public std::exception {
	string error;
	Position position;

public:

	
	MyException (string error = "", Position position = {0, 0});
	string what();
};

#endif