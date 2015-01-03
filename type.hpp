#ifndef TYPE
#define TYPE
#include "header.hpp"
#include "typenode.hpp"
#include "basicsymbol.hpp"


class Type: public BasicSymbol {
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

	string getName() const;
	int getSize() const;
	Type(string name, int size = 0);
	Type();
	Type(TypeNode*);

	vector<string>& getStorageModes();
	vector<string>& getModifiers();
	vector<string>& getAccessModes();
	vector<int>& getDimensions();

	const vector<string>& getFullName() const;
	string toString();

};


#endif