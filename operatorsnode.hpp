#ifndef OPERATORSNODE
#define OPERATORSNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"

class OperatorsNode: public BasicNode {
protected:
	vector<BasicNode*> children;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	vector<BasicNode*>& getChildren();
	void addOperator(BasicNode*);


};

#endif