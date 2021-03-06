#ifndef LEXEREXCEPTION
#define LEXEREXCEPTION

#include "myexception.hpp"
#include <string>

class LexerException : public MyException {
public:
	LexerException (std::string error = "", Position position = {0, 0});
};

#endif