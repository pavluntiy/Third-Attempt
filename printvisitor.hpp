#ifndef PRINTVISITOR
#define PRINTVISITOR

#include "basicvisitor.hpp"
#include "programnode.hpp"
#include "expressionnode.hpp"
#include "compoundnamenode.hpp"
#include "functioncallnode.hpp"
#include "typenode.hpp"
#include "vardeclarationnode.hpp"

#include "valuenode.hpp"

class PrintVisitor: public BasicVisitor{
protected:
	string shift;
	ostream *out;
public:
	virtual void visit(ProgramNode *node) override;
	virtual void visit(ExpressionNode *node) override;
	virtual void visit(TypeNode *node) override;
	virtual void visit(FunctionCallNode *node) override;
	virtual void visit(CompoundNameNode *node) override;
	virtual void visit(ValueNode *node) override;
	virtual void visit(VarDeclarationNode *node) override;

	PrintVisitor(ostream *out);

};


#endif