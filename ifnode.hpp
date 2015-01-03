#ifndef IFNODE
#define IFNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "functioncallnode.hpp"
#include "operatorsnode.hpp"

class IfNode: public BasicNode {
protected:
	BasicNode *condition;
	BasicNode *thenBranch;
	BasicNode *elseBranch;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	
	BasicNode* getCondition();
	BasicNode* getThenBranch();
	BasicNode* getElseBranch();

	IfNode(Position position);
	IfNode();


	void setCondition(BasicNode*);
	void setThenBranch(BasicNode*);
	void setElseBranch(BasicNode*);
};

#endif