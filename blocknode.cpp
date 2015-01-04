#include "blocknode.hpp"

BlockNode::BlockNode(){
	//cout << this->scope << "OH!";
}
BlockNode::BlockNode(Position position):BasicNode(position)
{
	this->child = nullptr;
	//cout << this->scope << "OH!";
}

	void BlockNode::accept(BasicVisitor *visitor){
		visitor->visit(this);
	}

	string BlockNode::toString(){
		return "Block";
	}

	void BlockNode::addChild(BasicNode *node){
		this->child = node;
	}

	BasicNode* BlockNode::getChild(){
		return this->child;
	}

