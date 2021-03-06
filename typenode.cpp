#include "typenode.hpp"

TypeNode::TypeNode(){}
TypeNode::TypeNode(Position position):BasicNode(position)
{

}

vector<string>& TypeNode::getStorageModes(){
	return this->storageModes;
}

vector<string>& TypeNode::getModifiers(){
	return this->modifiers;
}

vector<string>& TypeNode::getAccessModes(){
	return this->accessModes;
}

vector<ValueNode*>& TypeNode::getDimensions(){
	return this->dimensions;
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

void TypeNode::addDimension(ValueNode *what){
	this->dimensions.push_back(what);
}

void TypeNode::addName(CompoundNameNode *what){
	this->name = what;
}

// Type*  TypeNode::getTypeSymbol(){
// 	return this->type;
// }

// void TypeNode::setTypeSymbol(Type *type){
// 	this->type = type;
// }

string TypeNode::getMangledQualifiers(){
	stringstream result;


	for(auto it: storageModes){
		result << it << "_";
	}
	if(storageModes.size() > 0){
		result << "#";
	}

	for(auto it: modifiers){
		result << it << "_";
	}
	if(modifiers.size() > 0){
		result << "#";
	}

	for(auto it: accessModes){
		result << it << "_";
	}
	if(accessModes.size() > 0){
		result << "#";
	}

	for(auto it: dimensions){
		result << it << "_";
	}
	if(dimensions.size() > 0){
		result << "#";
	}
	return result.str();
}