#include "myexception.hpp"

	MyException::MyException (std::string error, Position position){
		this->error = error;
		this->position = position;
	}

	std::string MyException::what(){
		return this->error + " (" + this->position.toString() + ")";
	}
