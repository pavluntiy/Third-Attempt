#include "functioncallsymbol.hpp"

FunctionCallSymbol::FunctionCallSymbol(){
	function = nullptr;
}



void FunctionCallSymbol::addArgumentType(Type *type){
	this->argumentTypes.push_back(type);
}

void FunctionCallSymbol::setFunction(FunctionSymbol *function){
	//this->type = function->getType();
	this->function = function;
}

BasicSymbol* FunctionCallSymbol::getFunction(){
	return this->function;
}

vector<Type*> FunctionCallSymbol::getArgumentTypes(){
	return this->argumentTypes;
}

// Type* FunctionCallSymbol::getType(){
// 	//auto tmp = this->function->getType();
// 	//auto result = dynami_cast<Type*> (tmp->get
// 	return tmp;
// }

string FunctionCallSymbol::toString(string shift){
	stringstream result;

	//result << this->getFunction();
	if(this->getFunction()){
		result << shift << "'" << this->getFunction()->toString() << "' ";
	}
	else if(this->getFullName()){
		result << "'" << this->getFullName()->getSimpleName() << "'";
	}
	result << this->argumentsToString(shift) << '\n';

	return result.str();
}

// bool FunctionCallSymbol::exactlyEquals(FunctionType *candidate){
// 	auto otherArgs = candidate->getArguments();
// 	if(this->arguments.size() != otherArgs.size()){
// 		return false;
// 	}

// 	for(int i = 0; i < this->arguments.size(); ++i){
// 		if(this->arguments[i] != otherArgs[i]){
// 			return false;
// 		}
// 	}

// 	return true;
// }

const vector<FunctionSymbol*>& FunctionCallSymbol::getConversions(){
	return this->conversions;
}

// bool FunctionCallSymbol::conversionExists(FunctionType *candidate){
// 	auto otherArgs = candidate->getArguments();
// 	if(this->arguments.size() != otherArgs.size() && !candidate->isVarargs()){
// 		return false;
// 	}


// 	int bound = min(this->arguments.size(), otherArgs.size());
// 	vector<FunctionSymbol*> convertors;
// 	for(int i = 0; i < bound; ++i){
// 		if(this->arguments[i] == otherArgs[i] ){
// 			//cout << "Mimimi!\n";
// 			convertors.push_back(nullptr);
// 		}
// 		else {
// 			auto tmp = otherArgs[i]->getConversion(this->arguments[i]);
// 			//cout << this->arguments[i] << otherArgs[i];
// 			//cout << "ololo!";
// 			if(!tmp){
// 				return false;
// 			}
// 			else{
// 				convertors.push_back(tmp);
// 				//cout << "OLOLO";
// 			}
// 		}
// 	}

// 	this->conversions = convertors;


// 	return true;
// }

string FunctionCallSymbol::argumentsToString(string shift){
	stringstream result;


	result << shift << " arguments types:";
	for(auto it: this->argumentTypes){
	 	result << it->toString() << "; ";
	}
	return result.str();
}
