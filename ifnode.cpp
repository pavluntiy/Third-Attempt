#include "ifnode.hpp"

IfNode::IfNode(){
	this->thenBranch = nullptr;
	this->elseBranch = nullptr;
}
IfNode::IfNode(Position position):BasicNode(position)
{
	this->thenBranch = nullptr;
	this->elseBranch = nullptr;
}

void IfNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

string IfNode::toString(){
	return "if operator";
}

BasicNode* IfNode::getCondition(){
	return this->condition;
}

BasicNode* IfNode::getThenBranch(){
	return this->thenBranch;
}

BasicNode* IfNode::getElseBranch(){
	return this->elseBranch;
}


void IfNode::setCondition(BasicNode *what){
	this->condition = what;
}

void IfNode::setThenBranch(BasicNode *what){
	this->thenBranch = what;
}

void IfNode::setElseBranch(BasicNode*what){
	this->elseBranch = what;
}