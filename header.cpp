#include "header.hpp"

	Position::Position (int line, int linePosition): line(line), linePosition(linePosition){

	} 

	std::string Position::toString()
	const{
		std::stringstream ss;
		ss << "line: " << this->line << ", position: " << this->linePosition;
		return ss.str();
	}

template <typename T>
void myIgnore(T arg){
	volatile T tmp = arg;
	arg = tmp;
}
