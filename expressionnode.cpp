#include "expressionnode.hpp"

string ExpressionNode::toString(){
	return "Expression";
}

vector<BasicNode*>& ExpressionNode::getChildren(){
	return this->children;
}