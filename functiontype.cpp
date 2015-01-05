#include "functiontype.hpp"

string FunctionType::getName() const{
	return this->name;
}

int FunctionType::getSize() const{
	return this->size;
}

FunctionType::FunctionType(string name, int size){
	this->name = name;
	this->size = size;
}

FunctionType::FunctionType(){}

// FunctionType::FunctionType(SignatureNode *node){
// 	// this->name = node->getName()->getSimpleName();
// 	// this->storageModes = node->getStorageModes();
// 	// this->modifiers = node->getModifiers();
// 	// this->accessModes = node->getAccessModes();
// 	// this->fullName = node->getName()->getNames();
// 	// for(auto it: node->getDimensions()){
// 	// 	this->dimensions.push_back(stoi(it->getText()));
// 	// }
// }

void FunctionType::setReturnType(Type *type){
	this->returnType = type;
}

void FunctionType::addArgument(Type *type){
	this->arguments.push_back(type);
}


Type* FunctionType::getReturnType(){
	return this->returnType;
}

vector<Type*> FunctionType::getArguments(){
	return this->arguments;
}


string FunctionType::toString(){
	stringstream result;

	result << "'" << "function type" << "' ";
	result <<  "( returns " << this->getReturnType()->toString() << ") ";

	// result << "\nArguments:\n";
	// for(auto it: this->arguments){
	// 	result << it->toString();
	// }

	result << " at " << this;

	result << " declared at " << this->getPosition().toString();
	return result.str();
}

string FunctionType::argumentsToString(){
	stringstream result;


	result << "'" + this->getName() + "' arguments:";
	for(auto it: this->arguments){
	 	result << it->toString() << 	"; ";
	}
	return result.str();
}
