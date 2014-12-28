#include "functiondefinitionnode.hpp"

void FunctionDefinitionNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

string FunctionDefinitionNode::toString(){
	return "Function definition";
}

void FunctionDefinitionNode::setSignature(SignatureNode *what){
	this->signature = what;
}

void FunctionDefinitionNode::setOperators(OperatorsNode *what){
	this->operators = what;
}

SignatureNode* FunctionDefinitionNode::getSignature(){
	return this->signature;
}

OperatorsNode* FunctionDefinitionNode::getOperators(){
	return this->operators;
}