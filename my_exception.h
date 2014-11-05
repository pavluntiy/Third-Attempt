#ifndef MY_EXCEPTION
#define MY_EXCEPTION

#include <exception>
#include <string>

class MyException : public std::exception {
	std::string error;
public:

	enum class Type: int {
		DEFAULT,
		FATAL,
		NOTICE
	};

	Type type;

	std::string typeToString(){
		switch (this->type){
			case Type::DEFAULT : return "DEFAULT"; break;
			case Type::FATAL : return "FATAL"; break;
			case Type::NOTICE : return "NOTICE"; break;
		}

		return "DEFAULT";
	}
	MyException (std::string error, Type type = Type::DEFAULT){
		this->error = error;
		this->type = type;
	}
	std::string what(){
		return this->error;
	}

	Type getType(){
		return this->type;
	}
};

#endif