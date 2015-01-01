#include "basicscope.hpp"


// BasicScope::BasicScope(){}
// BasicScope::BasicScope(AbstractScope*){}

string BasicScope::getName(){
	return this->name;
}

void BasicScope::setName(string str){
	this->name = str;
}

void BasicScope::setParentScope(AbstractScope *parentScope){
	this->parentScope = parentScope;
}

AbstractScope* BasicScope::getParentScope(){
	return this->parentScope;
}

map<string, Type>& BasicScope::getTypes(){
	return this->types;
}

map<string, FunctionSymbol>& BasicScope::getFunctions(){
	return this->functions;
}

map<string, VariableSymbol>& BasicScope::getVariables(){
	return this->variables;
}

FunctionSymbol* BasicScope::resolveFunction(string name){
	AbstractScope *currentScope = this;
	while(currentScope != nullptr){
		if(currentScope->getFunctions().count(name)){
			return &currentScope->getFunctions()[name];
		}
		currentScope = currentScope->getParentScope();
	}
	throw NoticeException("Undeclared function '"+ name + "'!");
}

VariableSymbol* BasicScope::resolveVariable(string name){
	AbstractScope *currentScope = this;

	while(currentScope != nullptr){
		if(currentScope->getVariables().count(name)){
			return &currentScope->getVariables()[name];
		}
		currentScope = currentScope->getParentScope();
	}

	throw NoticeException("Undeclared function '"+ name + "'!");
}

Type* BasicScope::resolveType(string name){
	AbstractScope *currentScope = this;
	while(currentScope != nullptr){
		if(currentScope->getTypes().count(name)){
			return &currentScope->getTypes()[name];
		}
		currentScope = currentScope->getParentScope();
	}
	
	throw NoticeException("Undeclared type '"+ name + "'!");
}

Type* BasicScope::resolveType(Type type){
	string name = type.getName();
	AbstractScope *currentScope = this;
	while(currentScope != nullptr){
		if(currentScope->getTypes().count(name)){
			return &currentScope->getTypes()[name];
		}
		currentScope = currentScope->getParentScope();
	}
	
	throw NoticeException("Undeclared type'"+ name + "'!");
}


// void BasicScope::dump(ostream *out, string shift){}
// void BasicScope::declareFunction(FunctionSymbol function){}
// void BasicScope::declareVariable(VariableSymbol variable){}
// void BasicScope::declareType(Type type){}
