#include "programnode.hpp"

ProgramNode::ProgramNode(){}
ProgramNode::ProgramNode(Position position):BasicNode(position)
{

}

	void ProgramNode::accept(BasicVisitor *visitor){
		visitor->visit(this);
	}

	string ProgramNode::toString(){
		return "PROGRAM";
	}

	void ProgramNode::addChild(BasicNode *node){
		this->child = node;
	}

	BasicNode* ProgramNode::getChild(){
		return this->child;
	}

