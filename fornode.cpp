#include "fornode.hpp"

ForNode::ForNode(){}
ForNode::ForNode(Position position):BasicNode(position)
{

}

void ForNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

string ForNode::toString(){
	return "for operator";
}

BasicNode* ForNode::getInit(){
	return this->init;
}

BasicNode* ForNode::getCondition(){
	return this->condition;
}

BasicNode* ForNode::getStep(){
	return this->step;
}

BasicNode* ForNode::getAction(){
	return this->action;
}

void ForNode::setInit(BasicNode *what){
	this->init = what;
}

void ForNode::setCondition(BasicNode *what){
	this->condition = what;
}

void ForNode::setStep(BasicNode *what){
	this->step= what;
}

void ForNode::setAction(BasicNode *what){
	this->action = what;
}

