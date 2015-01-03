#include "operatorsnode.hpp"

OperatorsNode::OperatorsNode(){}
OperatorsNode::OperatorsNode(Position position):BasicNode(position)
{

}

void OperatorsNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

string OperatorsNode::toString(){
	return "Operators";
}

void OperatorsNode::addOperator(BasicNode* what){
	this->children.push_back(what);
}

vector<BasicNode*>& OperatorsNode::getChildren(){
	return this->children;
}