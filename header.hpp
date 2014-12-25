#ifndef HEADER
#define HEADER
#include <sstream>
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <set>
#include <fstream>

using namespace std;

struct Position {
	int line, linePosition;

	Position (int line = 0, int linePosition = 0);

	std::string toString() const;
};

template <typename T>
void myIgnore(T arg);

#endif