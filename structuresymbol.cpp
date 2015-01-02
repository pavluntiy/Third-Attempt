#include "structuresymbol.hpp"


int StructureSymbol::getOfsset(string name){
	return 200;
}
int StructureSymbol::getSize(){
	return 100500;
}

StructureSymbol::StructureSymbol(){}

string StructureSymbol::getName(){
	return this->name;
}

StructureSymbol::StructureSymbol(Type *type, AbstractScope *structureScope){
	this->name = type->getName();
	this->structureScope = structureScope;
	this->structureScope->setName(this->name);
	this->type = type;
}

AbstractScope* StructureSymbol::getStructureScope(){
	return this->structureScope;
}