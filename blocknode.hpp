#ifndef	BLOCKNODE
#define BLOCKNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"

class BlockNode: public BasicNode {

protected:
	BasicNode *child;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;
	string getType();

	BlockNode(Position position);
	BlockNode();

	BasicNode* getChild();
	void addChild(BasicNode *node);

};

#endif