
#include "vardeclarationnode.hpp"

void VarDeclarationNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

string VarDeclarationNode::toString(){
	return "Var Declaration";
}

void VarDeclarationNode::setType(TypeNode *type){
	this->type = type;
}

void VarDeclarationNode::addVariable(CompoundNameNode *what){
	this->variables.push_back(what);
}

TypeNode* VarDeclarationNode::getType(){
	return this->type;
}

vector<CompoundNameNode*>& VarDeclarationNode::getVariables(){
	return this->variables;
}

