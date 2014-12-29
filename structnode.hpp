#ifndef STRUCTNODE
#define STRUCTNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "functiondefinitionnode.hpp"
#include "signaturenode.hpp"
#include "vardeclarationnode.hpp"

class StructNode: public BasicNode {
protected:
	CompoundNameNode *name;
	vector<BasicNode*> functions;
	//vector<SignatureNode*> declarations;
	vector<BasicNode*> variables;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;


//	void addDefinition(FunctionDefinitionNode*);
//	void addDeclaration(SignatureNode*);
	void addFunction(BasicNode*);
	void addVariable(BasicNode*);
	void setName(CompoundNameNode*);

	CompoundNameNode* getName();
	vector<BasicNode*>& getFunctions();
	//vector<FunctionDefinitionNode*>& getDefinitions();
	//vector<SignatureNode*>& getDeclarations();
	vector<BasicNode*>& getVariables();
};

#endif