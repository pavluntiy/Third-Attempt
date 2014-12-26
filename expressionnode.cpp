#include "expressionnode.hpp"

string ExpressionNode::toString(){
	return "EXPRESSION";
}

vector<BasicNode*>& ExpressionNode::getChildren(){
	return this->children;
}