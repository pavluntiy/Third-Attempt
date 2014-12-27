#ifndef VARDECLARATIONNODE
#define VARDECLARATIONNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "typenode.hpp"
#include "compoundnamenode.hpp"

class VarDeclarationNode: public BasicNode {
protected:
	TypeNode *type;
	vector<CompoundNameNode*> variables;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	void setType(TypeNode *type);
	void addVariable(CompoundNameNode *what);

	TypeNode* getType();
	vector<CompoundNameNode*>& getVariables();

};

#endif