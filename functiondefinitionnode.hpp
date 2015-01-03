#ifndef FUNCTIONDEFINITIONNODE
#define FUNCTIONDEFINITIONNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "signaturenode.hpp"
#include "operatorsnode.hpp"

class FunctionDefinitionNode: public BasicNode {
protected:
	SignatureNode *signature;
	OperatorsNode *operators;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	
	SignatureNode* getSignature();
	OperatorsNode* getOperators();

	FunctionDefinitionNode(Position position);
	FunctionDefinitionNode();

	void setSignature(SignatureNode*);
	void setOperators(OperatorsNode*);
};

#endif