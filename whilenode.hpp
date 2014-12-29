#ifndef WHILENODE
#define WHILENODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "functioncallnode.hpp"
#include "operatorsnode.hpp"

class WhileNode: public BasicNode {
protected:
	BasicNode *condition;
	BasicNode *loop;
	BasicNode *elseBranch;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	
	BasicNode* getCondition();
	BasicNode* getLoop();
	BasicNode* getElseBranch();


	void setCondition(BasicNode*);
	void setLoop(BasicNode*);
	void setElseBranch(BasicNode*);
};

#endif