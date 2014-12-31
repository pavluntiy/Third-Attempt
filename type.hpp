#ifndef TYPE
#define TYPE
#include "header.hpp"


class Type{
protected:
	Type *parentType;

	vector<string> storageModes;
	vector<string> modifiers;
	vector<string> accessModes;
	vector<int> dimensions;

	string name;
	int size;

	map<Type, string> convertTo;

public:

	string getName();
	int getSize();

};

#endif