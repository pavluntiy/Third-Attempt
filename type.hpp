#ifndef TYPE
#define TYPE
#include "header.hpp"
#include "typenode.hpp"


class Type{
protected:
	//Type *parentType;

	vector<string> storageModes;
	vector<string> modifiers;
	vector<string> accessModes;
	vector<int> dimensions;

	vector<string> fullName;
	string name;
	int size;

	map<Type, string> convertTo;

public:

	string getName();
	int getSize();
	Type(string name, int size = 0);
	Type();
	Type(TypeNode*);

};

#endif