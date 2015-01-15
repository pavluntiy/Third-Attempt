#include "functioncallnode.hpp"

FunctionCallNode::FunctionCallNode(){}
FunctionCallNode::FunctionCallNode(Position position):BasicNode(position)
{

}

void FunctionCallNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}
	string FunctionCallNode::toString(){
		return "FUNCTION_CALL";
	}

	BasicNode* FunctionCallNode::getFunctionName(){
		return this->functionName;
	}
	vector<BasicNode*>& FunctionCallNode::getFunctionArgs(){
		return this->functionArgs;
	}


	void FunctionCallNode::addArgument(BasicNode *node){
		this->functionArgs.push_back(node);
	}


	FunctionCallNode::FunctionCallNode(BasicNode *functionName){
		this->functionName = functionName;
	}

	FunctionCallNode::FunctionCallNode(const Token &token){
		this->functionName = new CompoundNameNode(token);
		this->position = token.getPosition();
	}

void FunctionCallNode::replaceArgument(int index, BasicNode *node){
	this->functionArgs[index] = node;
}