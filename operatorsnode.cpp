#include "operatorsnode.hpp"

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