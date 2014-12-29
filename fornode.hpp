#ifndef FORNODE
#define FORNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "functioncallnode.hpp"
#include "operatorsnode.hpp"

class ForNode: public BasicNode {
protected:
	BasicNode *init;
	BasicNode *condition;
	BasicNode *step;
	BasicNode *action;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	
	BasicNode* getInit();
	BasicNode* getCondition();
	BasicNode* getStep();
	BasicNode* getAction();

	void setInit(BasicNode*);
	void setCondition(BasicNode*);
	void setStep(BasicNode*);
	void setAction(BasicNode*);
};

#endif