#ifndef HEADER
#define HEADER
#include <sstream>
using namespace std;

struct Position {
	int line, linePosition;

	Position (int line = 0, int linePosition = 0);

	std::string toString();
};

template <typename T>
void ignore(T arg);

#endif