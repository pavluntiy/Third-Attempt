#ifndef USINGNODE
#define USINGNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"

class UsingNode: public BasicNode {
protected:
	BasicNode *name;
	BasicNode *type;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	
	BasicNode* getType();
	BasicNode* getName();

	void setName(BasicNode*);
	void setType(BasicNode*);

	UsingNode(Position position);
	UsingNode();
};

#endif