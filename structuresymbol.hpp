#ifndef STRUCTURESYMBOL
#define STRUCTURESYMBOL

#include "header.hpp"
#include "type.hpp"
#include "basicsymbol.hpp"
#include "abstractscope.hpp"

class StructureSymbol: public BasicSymbol{
protected:
	AbstractScope *structureScope;
	Type *type;
	string name;
public:

	int getOfsset(string name);
	int getSize();
	string getName();

	StructureSymbol();
	StructureSymbol(Type *type, AbstractScope *scope);
	AbstractScope* getStructureScope();
	string toString();
};

#endif