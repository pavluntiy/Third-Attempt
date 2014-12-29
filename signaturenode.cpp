#include "signaturenode.hpp"

void SignatureNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

string SignatureNode::toString(){
	return "Signature";
}

void SignatureNode::setType(TypeNode *what){
	this->type = what;
}

void SignatureNode::setName(CompoundNameNode *what){
	this->name = what;
}

void SignatureNode::setVarargs(){
	this->varargs = true;
}

void SignatureNode::addArgument(SignatureNode::ArgumentType what){
	this->arguments.push_back(what);
}

TypeNode* SignatureNode::getType(){
	return this->type;
}

CompoundNameNode* SignatureNode::getName(){
	return this->name;
}

vector<SignatureNode::ArgumentType>& SignatureNode::getArguments(){
	return this->arguments;
}

bool SignatureNode::getVarargs(){
	return this->varargs;
}