#include "functioncallsymbol.hpp"

FunctionCallSymbol::FunctionCallSymbol(){
}

void FunctionCallSymbol::setReturnType(Type *type){
	this->returnType = type;
}

void FunctionCallSymbol::addArgument(Type *type){
	this->arguments.push_back(type);
}

Type* FunctionCallSymbol::getReturnType(){
	return this->returnType;
}

vector<Type*> FunctionCallSymbol::getArguments(){
	return this->arguments;
}

string FunctionCallSymbol::toString(string shift){
	stringstream result;

	result << shift << "'" << this->getName() << "' ";
	result <<  "( type " << this->getType()->toString() << ") ";

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


	result << shift << "'" + this->getName() + "' arguments:";
	for(auto it: this->arguments){
	 	result << it->toString() << 	"; ";
	}
	return result.str();
}