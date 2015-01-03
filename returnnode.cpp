#include "returnnode.hpp"

ReturnNode::ReturnNode(){}
ReturnNode::ReturnNode(Position position):BasicNode(position)
{

}


void ReturnNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

string ReturnNode::toString(){
	return "Return";
}

void ReturnNode::setResult(BasicNode *what){
	this->result = what;
}

void ReturnNode::setName(string what){
	this->name = what;
}


string ReturnNode::getName(){
	return this->name;
}


BasicNode* ReturnNode::getResult(){
	return this->result;
}