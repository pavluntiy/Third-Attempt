#include "functioncallsymbol.hpp"

FunctionCallSymbol::FunctionCallSymbol(){
	function = nullptr;
}



void FunctionCallSymbol::addArgument(Type *type){
	this->arguments.push_back(type);
}

void FunctionCallSymbol::setFunction(FunctionSymbol *function){
	this->type = function->getReturnType();
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

	result << shift << "'" << this->getFunction()->toString() << "' ";
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

const vector<FunctionSymbol*>& FunctionCallSymbol::getConversions(){
	return this->conversions;
}

bool FunctionCallSymbol::conversionExists(FunctionSymbol *candidate){
	auto otherArgs = candidate->getArguments();
	if(this->arguments.size() != otherArgs.size() && !candidate->isVarargs()){
		return false;
	}

	int bound = min(this->arguments.size(), otherArgs.size());
	vector<FunctionSymbol*> convertors;
	for(int i = 0; i < bound; ++i){
		if(this->arguments[i] == otherArgs[i] ){
			convertors.push_back(nullptr);
		}
		else {
			auto tmp = otherArgs[i]->getConversion(this->arguments[i]);
			//cout << this->arguments[i] << otherArgs[i];
			//cout << "ololo!";
			if(!tmp){
				return false;
			}
			else{
				convertors.push_back(tmp);
				//cout << "OLOLO";
			}
		}
	}

	this->conversions = convertors;

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