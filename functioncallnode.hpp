#ifndef FUNCTIONCALLNODE
#define FUNCTIONCALLNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "functioncallnode.hpp"

class FunctionCallNode: public BasicNode {

friend class BasicVisitor;
protected:
	ExpressionNode *functionName;
	vector<ExpressionNode*> functionArgs;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	ExpressionNode* getFunctionName();
	vector<ExpressionNode*>& getFunctionArgs();

	void addArg(ExpressionNode *node);

};

#endif