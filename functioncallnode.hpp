#ifndef FUNCTIONCALLNODE
#define FUNCTIONCALLNODE

#include "token.hpp"
#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "functioncallnode.hpp"
#include "compoundnamenode.hpp"

class FunctionCallNode: public BasicNode {
protected:
	BasicNode *functionName;
	vector<BasicNode*> functionArgs;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	BasicNode* getFunctionName();
	vector<BasicNode*>& getFunctionArgs();

	string transformOperators(const Token &token);

	void addArg(BasicNode *node);

	FunctionCallNode();
	FunctionCallNode(BasicNode *functionName);
	FunctionCallNode(const Token &token);

};

#endif