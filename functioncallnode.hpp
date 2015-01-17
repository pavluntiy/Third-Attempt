#ifndef FUNCTIONCALLNODE
#define FUNCTIONCALLNODE

#include "token.hpp"
#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "functioncallnode.hpp"
#include "compoundnamenode.hpp"

class FunctionCallNode: public BasicNode {
protected:
	CompoundNameNode *functionName;
	vector<BasicNode*> functionArgs;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	BasicNode* getFunctionName();
	vector<BasicNode*>& getFunctionArgs();

	string transformOperators(const Token &token);

	void addArgument(BasicNode *node);

	FunctionCallNode(Position position);
	FunctionCallNode();
	FunctionCallNode(BasicNode *functionName);
	FunctionCallNode(const Token &token);

	void replaceArgument(int, BasicNode*);

};

#endif