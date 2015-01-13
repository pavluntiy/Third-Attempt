#include "functionsymbol.hpp"

FunctionSymbol::FunctionSymbol(){
	this->defined = false;
}

void FunctionSymbol::setReturnType(Type *type){
	this->returnType = type;
}

void FunctionSymbol::addArgument(Type *type){
	this->arguments.push_back(type);
}

void FunctionSymbol::setFunctionScope(AbstractScope *scope){
	this->functionScope = scope;
}

void FunctionSymbol::setFunctionLabel(string str){
	this->functionLabel = str;
}

Type* FunctionSymbol::getReturnType(){
	return this->returnType;
	//return this->functionType->getReturnType();
}

vector<Type*> FunctionSymbol::getArguments(){
	return this->arguments;
	//return this->functionType->getArguments();
}

AbstractScope* FunctionSymbol::getFunctionScope(){
	return this->functionScope;
}

string FunctionSymbol::getFunctionLabel(){
	return this->functionLabel;
}

string FunctionSymbol::toString(){
	stringstream result;

	result << "'" << this->getName() << "' ";
	//result <<  "( returns " << this->getReturnType()->toString() << ") ";
	result <<  "( type " << this->getType()->toString() << ") ";

	// result << "\nArguments:\n";
	// for(auto it: this->arguments){
	// 	result << it->toString();
	// }

	result << " at " << this;

		result << " declared at " << this->getPosition().toString();
	if(!this->isOnlyDeclared()){
		result << " and defined at " << this->getDefinitionPosition().toString();
	}
	return result.str();
}

string FunctionSymbol::argumentsToString(){
	stringstream result;


	result << "'" + this->getName() + "' arguments:";
	for(auto it: this->arguments){
	 	result << it->toString() << 	"; ";
	}
	return result.str();
}

bool FunctionSymbol::isOnlyDeclared(){
	return !this->defined;
}

void FunctionSymbol::define(Position position){
	this->definitionPosition = position;
	this->defined = true;
}

Position FunctionSymbol::getDefinitionPosition(){
	return this->definitionPosition;
}

void FunctionSymbol::setDefinitionPosition(Position position){
	this->definitionPosition = position;
}