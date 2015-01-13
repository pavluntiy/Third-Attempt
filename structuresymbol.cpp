#include "structuresymbol.hpp"


int StructureSymbol::getOfsset(string name){
	return 200;
}
int StructureSymbol::getSize(){
	return 100500;
}

StructureSymbol::StructureSymbol(){
	this->defined = false;
}

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

string StructureSymbol::toString(string shift){
	stringstream result;
	result << "structure '" << this->getName() <<"', defining type " << this->type->toString();
	return result.str();
}

bool StructureSymbol::isOnlyDeclared(){
	return !this->defined;
}

void StructureSymbol::define(Position position){
	this->definitionPosition = position;
	this->defined = true;
}

Position StructureSymbol::getDefinitionPosition(){
	return this->definitionPosition;
}

void StructureSymbol::setDefinitionPosition(Position position){
	this->definitionPosition = position;
}

Type* StructureSymbol::getStructureType(){
	return this->type;
}