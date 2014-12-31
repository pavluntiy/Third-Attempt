#ifndef TYPE
#define TYPE
#include "header.hpp"
#include "typenode.hpp"


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

	Type(string name, int size);
	Type();
	Type(TypeNode*);

};

#endif