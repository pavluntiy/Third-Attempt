#include "functioncallnode.hpp"

void FunctionCallNode::accept(BasicVisitor *visitor){
	//visitor->visit(this);
}
	string FunctionCallNode::toString(){
		return "FUNCTION_CALL";
	}

	ExpressionNode* FunctionCallNode::getFunctionName(){
		return this->functionName;
	}
	vector<ExpressionNode*>& FunctionCallNode::getFunctionArgs(){
		return this->functionArgs;
	}

	void FunctionCallNode::addArg(ExpressionNode *node){
		this->functionArgs.push_back(node);
	}