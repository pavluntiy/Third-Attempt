#ifndef	PROGRAMNODE
#define PROGRAMNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"

class ProgramNode: public BasicNode {

friend class BasicVisitor;
protected:
	BasicNode *child;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;
	string getType();

	BasicNode* getChild();
	void addChild(BasicNode *node);

};

#endif