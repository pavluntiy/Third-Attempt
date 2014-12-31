#include "type.hpp"

string Type::getName(){
	return this->name;
}

int Type::getSize(){
	return this->size;
}

Type::Type(string name, int size){
	this->name = name;
	this->size = size;
}
Type::Type(){}

Type::Type(TypeNode *node){
	this->name = node->getName()->getNames()[0];
	this->storageModes = node->getStorageModes();
	this->modifiers = node->getModifiers();
	this->accessModes = node->getAccessModes();
	for(auto it: node->getDimensions()){
		this->dimensions.push_back(stoi(it->getText()));
	}
}