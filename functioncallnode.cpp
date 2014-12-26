#include "functioncallnode.hpp"

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


	void FunctionCallNode::addArg(BasicNode *node){
		this->functionArgs.push_back(node);
	}

	FunctionCallNode::FunctionCallNode(){

	}

	FunctionCallNode::FunctionCallNode(BasicNode *functionName){
		this->functionName = functionName;
	}

	FunctionCallNode::FunctionCallNode(Token token){
		this->functionName = new CompoundNameNode(token.toString());
		//cout << "OK!";
	}