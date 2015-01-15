#include "functioncallsymbol.hpp"

FunctionCallSymbol::FunctionCallSymbol(){
	function = nullptr;
}



void FunctionCallSymbol::addArgument(Type *type){
	this->arguments.push_back(type);
}

void FunctionCallSymbol::setFunction(FunctionSymbol *function){
	this->function = function;
}

FunctionSymbol* FunctionCallSymbol::getFunction(){
	return this->function;
}

vector<Type*> FunctionCallSymbol::getArguments(){
	return this->arguments;
}

Type* FunctionCallSymbol::getType(){
	return this->function->getReturnType();
}

string FunctionCallSymbol::toString(string shift){
	stringstream result;

	result << shift << "'" << this->getFullName()->getSimpleName() << "' ";
	result << this->argumentsToString(shift) << '\n';

	return result.str();
}

bool FunctionCallSymbol::exactlyEquals(FunctionSymbol *candidate){
	auto otherArgs = candidate->getArguments();
	if(this->arguments.size() != otherArgs.size()){
		return false;
	}

	for(int i = 0; i < this->arguments.size(); ++i){
		if(this->arguments[i] != otherArgs[i]){
			return false;
		}
	}

	return true;
}

string FunctionCallSymbol::argumentsToString(string shift){
	stringstream result;


	result << shift << " arguments:";
	for(auto it: this->arguments){
	 	result << it->toString() << "; ";
	}
	return result.str();
}