#include "structnode.hpp"

StructNode::StructNode(){}
StructNode::StructNode(Position position):BasicNode(position)
{

}

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

void StructNode::addStructure(BasicNode *what){
	this->structures.push_back(what);
}

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

vector<BasicNode*>& StructNode::getStructures(){
	return this->structures;
}

// vector<SignatureNode*>& StructNode::getDeclarations(){
// 	return this->declarations;
// }

vector<BasicNode*>& StructNode::getVariables(){
	return this->variables;
}