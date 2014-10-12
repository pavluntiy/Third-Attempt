#ifndef DATA_EXCEPTION
#define DATA_EXCEPTION

#include <exception>
#include <string>

class DataException : public std::exception {
	std::string error;
public:
	DataException (std::string error){
		this->error = error;
	}
	std::string what(){
		return this->error;
	}
};

#endif