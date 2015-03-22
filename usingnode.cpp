#include "usingnode.hpp"

UsingNode::UsingNode(){}
UsingNode::UsingNode(Position position):BasicNode(position)
{

}


void UsingNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

string UsingNode::toString(){
	return "using";
}

void UsingNode::setType(BasicNode *what){
	this->type = what;
}

void UsingNode::setName(BasicNode *what){
	this->name = what;
}


BasicNode* UsingNode::getName(){
	return this->name;
}


BasicNode* UsingNode::getType(){
	return this->type;
}