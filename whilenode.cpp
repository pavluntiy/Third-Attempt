#include "whilenode.hpp"

WhileNode::WhileNode(){
	this->loop = nullptr;
	this->elseBranch = nullptr;
}
WhileNode::WhileNode(Position position):BasicNode(position)
{
	this->loop = nullptr;
	this->elseBranch = nullptr;
}


void WhileNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

string WhileNode::toString(){
	return "while operator";
}

BasicNode* WhileNode::getCondition(){
	return this->condition;
}

BasicNode* WhileNode::getLoop(){
	return this->loop;
}

BasicNode* WhileNode::getElseBranch(){
	return this->elseBranch;
}


void WhileNode::setCondition(BasicNode *what){
	this->condition = what;
}

void WhileNode::setLoop(BasicNode *what){
	this->loop = what;
}

void WhileNode::setElseBranch(BasicNode*what){
	this->elseBranch = what;
}