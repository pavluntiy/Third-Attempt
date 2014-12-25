#include "myexception.hpp"

	MyException::MyException (std::string error){
		this->error = error;
	}

	std::string MyException::what(){
		return this->error;
	}
