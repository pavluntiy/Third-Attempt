#ifndef VARDECLARATIONNODE
#define VARDECLARATIONNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "typenode.hpp"
#include "compoundnamenode.hpp"

class VarDeclarationNode: public BasicNode {
public: 
	typedef pair<CompoundNameNode*, BasicNode* > VariableListType;
protected:
	TypeNode *type;
	vector<VariableListType> variables;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	void setType(TypeNode *type);
	void addVariable(VariableListType);

	TypeNode* getType();
	vector<VariableListType>& getVariables();

	VarDeclarationNode(Position position);
	VarDeclarationNode();

};

#endif