#ifndef IMPORTNODE
#define IMPORTNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "compoundnamenode.hpp"

class ImportNode: public BasicNode {
protected:
	
	CompoundNameNode *moduleName;
	CompoundNameNode *scopeName;
	BasicNode *tree;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	
	CompoundNameNode *getModuleName();
	CompoundNameNode *getScopeName();

	BasicNode *getTree();
	void setTree(BasicNode *tree);

	void setModuleName(CompoundNameNode*);
	void setScopeName(CompoundNameNode*);

	ImportNode(Position position);
	ImportNode();
};

#endif