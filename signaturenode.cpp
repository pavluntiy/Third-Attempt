#include "signaturenode.hpp"

SignatureNode::SignatureNode(){}
SignatureNode::SignatureNode(Position position):BasicNode(position)
{

}

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

FunctionSymbol* SignatureNode::getFunctionSymbol(){
	return this->functionSymbol;
}

void SignatureNode::setFunctionSymbol(FunctionSymbol *functionSymbol){
	this->functionSymbol = functionSymbol;
}

void SignatureNode::setConstruct(){
	this->functionType = SpecialType::CONSTRUCT;
}

void SignatureNode::setDestruct(){
	this->functionType = SpecialType::DESTRUCT;
}

bool SignatureNode::isConstruct(){
	return this->functionType == SpecialType::CONSTRUCT;
}

bool SignatureNode::isDestruct(){
		return this->functionType == SpecialType::DESTRUCT;
}