#include "structuresymbol.hpp"


int StructureSymbol::getOfsset(string name){

}
int StructureSymbol::getSize(){
	return 100500;
}

StructureSymbol::StructureSymbol(){}

string StructureSymbol::getName(){
	return this->name;
}

StructureSymbol::StructureSymbol(Type *type, AbstractScope *scope){
	this->name = type->getName();
	this->type = type;
	this->structureScope = scope;
}

AbstractScope* StructureSymbol::getStructureScope(){
	return this->structureScope;
}