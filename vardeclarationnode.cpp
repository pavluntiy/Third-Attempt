
#include "vardeclarationnode.hpp"

VarDeclarationNode::VarDeclarationNode(){}
VarDeclarationNode::VarDeclarationNode(Position position):BasicNode(position)
{

}

void VarDeclarationNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}

string VarDeclarationNode::toString(){
	return "Var Declaration";
}

void VarDeclarationNode::setType(TypeNode *type){
	this->type = type;
}

void VarDeclarationNode::addVariable(VarDeclarationNode::VariableListType what){
	this->variables.push_back(what);
}

TypeNode* VarDeclarationNode::getType(){
	return this->type;
}

vector<VarDeclarationNode::VariableListType>& VarDeclarationNode::getVariables(){
	return this->variables;
}

