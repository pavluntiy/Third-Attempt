#ifndef RETURNNODE
#define RETURNNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"

class ReturnNode: public BasicNode {
protected:
	string name;
	BasicNode *result;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	
	BasicNode* getResult();
	string getName();

	void setName(string);
	void setResult(BasicNode*);

	ReturnNode(Position position);
	ReturnNode();
};

#endif