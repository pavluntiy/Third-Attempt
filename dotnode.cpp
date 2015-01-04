#include "dotnode.hpp"


void DotNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

DotNode::DotNode(){
	this->left = nullptr;
	this->right = nullptr;
}

DotNode::DotNode(Position position):BasicNode(position)
{
	this->left = nullptr;
	this->right = nullptr;
}

DotNode::DotNode(Token token):BasicNode(token.getPosition())
{	
	this->name = token.getText();
	this->left = nullptr;
	this->right = nullptr;
}


string DotNode::toString(){
	return this->name;
}

BasicNode* DotNode::getLeft(){
	return this->left;
}

BasicNode* DotNode::getRight(){
	return this->right;
}

void DotNode::setLeft(BasicNode *left){
	this->left = left;
}

void DotNode::setRight(BasicNode *right){
	this->right = right;
}