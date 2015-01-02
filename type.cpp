#include "type.hpp"

string Type::getName() const{
	return this->name;
}

int Type::getSize() const{
	return this->size;
}

Type::Type(string name, int size){
	this->name = name;
	this->size = size;
}
Type::Type(){}

Type::Type(TypeNode *node){
	this->name = node->getName()->getSimpleName();
	this->storageModes = node->getStorageModes();
	this->modifiers = node->getModifiers();
	this->accessModes = node->getAccessModes();
	this->fullName = node->getName()->getNames();
	for(auto it: node->getDimensions()){
		this->dimensions.push_back(stoi(it->getText()));
	}
}
const vector<string>& Type::getFullName()const {
	return this->fullName;
}

vector<string>& Type::getStorageModes(){
	return this->storageModes;
}

vector<string>& Type::getModifiers(){
	return this->modifiers;
}

vector<string>& Type::getAccessModes(){
	return this->accessModes;
}

vector<int>& Type::getDimensions(){
	return this->dimensions;
}


string Type::toString(){
	stringstream result;
	for(auto it: this->getStorageModes()){
		result << it + " ";
	}
	for(auto it: this->getModifiers()){
		result << it + " ";
	}
	for(auto it: this->getAccessModes()){
		result <<  it + " ";
	}

	for(auto it: this->getDimensions()){
		result << to_string(it) + " ";
	}

	result << "'" << this->getName() << "'";
	result << " at " << this;
	//auto tmp = ;
	return result.str();
}