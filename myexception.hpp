#ifndef MYEXCEPTION
#define MYEXCEPTION

#include <exception>
#include <string>

class MyException : public std::exception {
	std::string error;
public:

	
	MyException (std::string error);
	std::string what();
};

#endif