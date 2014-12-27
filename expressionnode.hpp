#ifndef EXPRESSIONNODE
#define EXPRESSIONNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"

class ExpressionNode: public BasicNode {
protected:
	vector<BasicNode*> children;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	vector<BasicNode*>& getChildren();


};

#endif