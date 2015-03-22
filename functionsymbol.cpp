#include "functionsymbol.hpp"

FunctionSymbol::FunctionSymbol(){
	this->defined = false;
}

void FunctionSymbol::setFunctionType(FunctionType *type){
	this->functionType = type;
}

void FunctionSymbol::setReturnType(Type *type){
	this->returnType = type;
}

void FunctionSymbol::addArgumentType(Type *type){
	this->argumentTypes.push_back(type);
}

void FunctionSymbol::setFunctionScope(AbstractScope *scope){
	this->functionScope = scope;
}

void FunctionSymbol::setFunctionLabel(string str){
	this->functionLabel = str;
}

FunctionType* FunctionSymbol::getFunctionType(){
	return dynamic_cast<FunctionType*>(this->type);
	//return this->functionType->getReturnType();
}


Type* FunctionSymbol::getReturnType(){
	return this->returnType;
	//return this->functionType->getReturnType();
}

vector<Type*> FunctionSymbol::getArgumentTypes(){
	return this->argumentTypes;
	//return this->functionType->getArguments();
}

AbstractScope* FunctionSymbol::getFunctionScope(){
	return this->functionScope;
}

string FunctionSymbol::getFunctionLabel(){
	return this->functionLabel;
}

bool FunctionSymbol::isVarargs(){
	return this->varargs;
}

void FunctionSymbol::setVarargs(){
	this->varargs = true;
}

string FunctionSymbol::toString(string shift){
	stringstream result;

	result << shift << "'" << this->getName() << "' ";
	result <<  "( type " << this->getType()->toString() << ") ";

	// result << "\nArguments:\n";
	// for(auto it: this->arguments){
	// 	result << it->toString();
	// }

	result << " at " << this;

		result << shift << " declared at " << this->getPosition().toString();
	if(!this->isOnlyDeclared()){
		result << shift << " and defined at " << this->getDefinitionPosition().toString();
	}
	return result.str();
}

string FunctionSymbol::argumentsToString(string shift){
	stringstream result;


	result << shift << "'" + this->getName() + "' arguments types:";
	for(auto it: this->argumentTypes){
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

bool FunctionSymbol::exactlyEquals(FunctionSymbol *candidate){
	auto candidateArgTypes = candidate->getArgumentTypes();
	if(this->argumentTypes.size() != candidateArgTypes.size()){
		return false;
	}

	for(int i = 0; i < this->argumentTypes.size(); ++i){
		if(this->argumentTypes[i] != candidateArgTypes[i]){
			return false;
		}
	}

	return true;
}

