#ifndef	PROGRAMNODE
#define PROGRAMNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"

class ProgramNode: public BasicNode {

friend class BasicVisitor;
protected:
	BasicNode *child;
	BasicNode *autoImport;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;
	string getType();

	ProgramNode(Position position);
	ProgramNode();

	BasicNode* getChild();
	void addChild(BasicNode *node);

	BasicNode* getAutoImport();
	void setAutoImport(BasicNode *node);

};

#endif