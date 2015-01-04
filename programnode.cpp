#include "programnode.hpp"

ProgramNode::ProgramNode(){}
ProgramNode::ProgramNode(Position position):BasicNode(position)
{
	this->child = nullptr;
}

	void ProgramNode::accept(BasicVisitor *visitor){
		visitor->visit(this);
	}

	string ProgramNode::toString(){
		return "Program";
	}

	void ProgramNode::addChild(BasicNode *node){
		this->child = node;
	}

	BasicNode* ProgramNode::getChild(){
		return this->child;
	}

