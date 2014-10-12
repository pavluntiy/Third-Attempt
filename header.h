#ifndef HEADER
#define HEADER
#include <sstream>

struct Position {
	int line, linePosition;

	Position (int line = 0, int linePosition = 0): line(line), linePosition(linePosition){

	} 

	std::string toString()
	const{
		std::stringstream ss;
		ss << "line: " << this->line << ", position: " << this->linePosition;
		return ss.str();
	}
};

template <typename T>
void ignore(T arg){
	volatile T tmp = arg;
};

#endif