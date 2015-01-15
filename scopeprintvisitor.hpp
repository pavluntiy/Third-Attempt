#ifndef	SCOPEPRINTVISITOR
#define SCOPEPRINTVISITOR

#include "basicvisitor.hpp"

#include "programnode.hpp"
#include "blocknode.hpp"
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
#include "dotnode.hpp"

#include "noticeexception.hpp"
#include "type.hpp"
#include "functionsymbol.hpp"
#include "functioncallsymbol.hpp"
#include "valuesymbol.hpp"

#include "functionscope.hpp"
#include "globalscope.hpp"
#include "structurescope.hpp"
#include "localscope.hpp"
#include "structuresymbol.hpp"


class ScopePrintVisitor: public BasicVisitor{
protected:
	AbstractScope *currentScope;
	string shift;
	ostream *out;
	stack<AbstractScope*> scopes;
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

	ScopePrintVisitor(ostream *out);
	virtual ~ScopePrintVisitor() override;

	void setCurrentScope(AbstractScope*);
	void restoreCurrentScope();
};


#endif