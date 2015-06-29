#include "importnode.hpp"

ImportNode::ImportNode(){}
ImportNode::ImportNode(Position position):BasicNode(position)
{

}


void ImportNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

string ImportNode::toString(){
	auto result = "import ";
	// result += this->moduleName->toString();
	// if(this->scopeName){
	// 	result += " to " + this->scopeName->toString();
	// }
	return result;
}

BasicNode* ImportNode::getTree(){
	return this->tree;
}

void ImportNode::setTree(BasicNode *tree){
	this->tree = tree;
}

CompoundNameNode* ImportNode::getModuleName(){
	return this->moduleName;
}

CompoundNameNode* ImportNode::getScopeName(){
	return this->scopeName;
}

void ImportNode::setModuleName(CompoundNameNode *moduleName){
	this->moduleName = moduleName;
}

void ImportNode::setScopeName(CompoundNameNode *scopeName){
	this->scopeName = scopeName;
}