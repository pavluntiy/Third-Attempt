#ifndef PRINTVISITOR
#define PRINTVISITOR

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
#include "blocknode.hpp"
#include "dotnode.hpp"
#include "importnode.hpp"

#include "valuenode.hpp"
#include "usingnode.hpp"

class PrintVisitor: public BasicVisitor{
protected:
	string shift;
	ostream *out;
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
	virtual void visit(BlockNode *node) override;
	virtual void visit(DotNode *node) override;
	virtual void visit(ImportNode *node) override;
	virtual void visit(UsingNode *node) override;


	PrintVisitor(ostream *out);
	virtual ~PrintVisitor() override;
};


#endif