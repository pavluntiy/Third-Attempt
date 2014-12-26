#ifndef TYPENODE
#define TYPENODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"

class TypeNode: public BasicNode {

friend class BasicVisitor;
protected:
	vector<string> storageModes;
	vector<string> modifiers;
	vector<string> accessModes;
	CompoundNameNode *name;


public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	vector<BasicNode*>& getChildren();


};

#endif