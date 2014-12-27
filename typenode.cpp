#include "typenode.hpp"

vector<string>& TypeNode::getStorageModes(){
	return this->storageModes;
}

vector<string>& TypeNode::getModifiers(){
	return this->modifiers;
}

vector<string>& TypeNode::getAccessModes(){
	return this->accessModes;
}

CompoundNameNode* TypeNode::getName(){
	return this->name;
}

string TypeNode::toString(){
	return "Type";
}

void TypeNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

void TypeNode::addStorageMode(string what){
	this->storageModes.push_back(what);
}
void TypeNode::addModifier(string what){
	this->modifiers.push_back(what);
}
void TypeNode::addAccessMode(string what){
	this->accessModes.push_back(what);
}

void TypeNode::addName(CompoundNameNode *what){
	this->name = what;
}