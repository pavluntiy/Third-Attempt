#include "basicsymbol.hpp"


void BasicSymbol::setName(string name){
	this->name = name;
}

void BasicSymbol::setType(Type *type){
	this->type = type;
}

void BasicSymbol::setDefinitionScope(BasicScope *scope){
	this->definitionScope = scope;
}

string BasicSymbol::getName() const{
	return this->name;
}

Type* BasicSymbol::getType(){
	return this->type;
}

BasicScope* BasicSymbol::getDefinitionScope(){
	return this->definitionScope;
}

void BasicSymbol::setPosition(Position position){
	this->position = position;
}

Position BasicSymbol::getPosition(){
	return this->position;
}

BasicSymbol::BasicSymbol(Position position){
	this->position = position;
}

CompoundNameNode* BasicSymbol::getFullName(){
	return this->fullName;
}

void BasicSymbol::setFullName(CompoundNameNode *name){
	this->fullName = name;
}



