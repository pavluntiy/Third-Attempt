#include "functionsymbol.hpp"


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
}

vector<Type*> FunctionSymbol::getArguments(){
	return this->arguments;
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
	result <<  "( returns " << this->getReturnType()->toString() << ") ";

	// result << "\nArguments:\n";
	// for(auto it: this->arguments){
	// 	result << it->toString();
	// }

	result << " at " << this;
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