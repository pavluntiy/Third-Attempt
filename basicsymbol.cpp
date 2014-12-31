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

string BasicSymbol::getName(){
	return this->name;
}

Type* BasicSymbol::getType(){
	return this->type;
}

BasicScope* BasicSymbol::getDefinitionScope(){
	return this->definitionScope;
}