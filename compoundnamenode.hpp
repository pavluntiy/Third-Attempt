#ifndef COMPOUNDNAMENODE
#define COMPOUNDNAMENODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"

class CompoundNameNode: public BasicNode {

friend class BasicVisitor;
protected:
	string left;
	CompoundNameNode *right;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	string getLeft();
	CompoundNameNode* getRight();

	CompoundNameNode(string left = "", CompoundNameNode *right = nullptr);

};

#endif