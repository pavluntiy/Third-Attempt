#ifndef LEXER_EXCEPTION
#define LEXER_EXCEPTION

#include <exception>
#include <string>

class LexerException : public std::exception {
	std::string error;
public:
	LexerException (std::string error){
		this->error = error;
	}
	std::string what(){
		return this->error;
	}
};

#endif