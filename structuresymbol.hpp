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
	Position definitionPosition;
	bool defined;
public:

	int getOfsset(string name);
	int getSize();
	string getName();

	StructureSymbol();
	StructureSymbol(Type *type, AbstractScope *scope);
	AbstractScope* getStructureScope();
	string toString();

	bool isOnlyDeclared();
	void define(Position position = {0, 0});

	Position getDefinitionPosition();
	void setDefinitionPosition(Position position);
	Type* getStructureType();
};

#endif