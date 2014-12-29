#include "structnode.hpp"

void StructNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

string StructNode::toString(){
	return "Struct";
}

void StructNode::addFunction(BasicNode *what){
	this->functions.push_back(what);
}

// void StructNode::addDeclaration(SignatureNode *what){
// 	this->declarations.push_back(what);
// }

void StructNode::addVariable(BasicNode *what){
	this->variables.push_back(what);
}

void StructNode::setName(CompoundNameNode *what){
	this->name = what;
}

CompoundNameNode* StructNode::getName(){
	return this->name;
}

vector<BasicNode*>& StructNode::getFunctions(){
	return this->functions;
}

// vector<SignatureNode*>& StructNode::getDeclarations(){
// 	return this->declarations;
// }

vector<BasicNode*>& StructNode::getVariables(){
	return this->variables;
}