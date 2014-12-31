#include "functionsymbol.hpp"


void FunctionSymbol::setReturnType(Type *type){
	this->returnType = type;
}

void FunctionSymbol::addArgument(Type *type){
	this->arguments.push_back(type);
}

void FunctionSymbol::setFunctionScope(BasicScope *scope){
	this->functionScope = scope;
}

void FunctionSymbol::setFunctionLabel(string str){
	this->functionLabel = str;
}

Type* FunctionSymbol::getReturnType(){
	return this->returnType;
}

vector<Type*> FunctionSymbol::getArguments(){
	return this->arguments;
}

BasicScope* FunctionSymbol::getFunctionScope(){
	return this->functionScope;
}

string FunctionSymbol::getFunctionLabel(){
	return this->functionLabel;
}