#ifndef	DELETEVISITOR
#define DELETEVISITOR

#include "basicvisitor.hpp"

#include "programnode.hpp"
#include "operatorsnode.hpp"
#include "compoundnamenode.hpp"
#include "functioncallnode.hpp"
#include "typenode.hpp"
#include "vardeclarationnode.hpp"
#include "signaturenode.hpp"
#include "functiondefinitionnode.hpp"
#include "ifnode.hpp"
#include "whilenode.hpp"
#include "returnnode.hpp"
#include "fornode.hpp"
#include "structnode.hpp"

#include "valuenode.hpp"

class DeleteVisitor: public BasicVisitor{
public:
	virtual void visit(ProgramNode *node) override;
	virtual void visit(OperatorsNode *node) override;
	virtual void visit(TypeNode *node) override;
	virtual void visit(FunctionCallNode *node) override;
	virtual void visit(CompoundNameNode *node) override;
	virtual void visit(ValueNode *node) override;
	virtual void visit(VarDeclarationNode *node) override;
	virtual void visit(SignatureNode *node) override;
	virtual void visit(FunctionDefinitionNode *node) override;
	virtual void visit(IfNode *node) override;
	virtual void visit(WhileNode *node) override;
	virtual void visit(ForNode *node) override;
	virtual void visit(ReturnNode *node) override;
	virtual void visit(StructNode *node) override;

	virtual ~DeleteVisitor() override;
};


#endif