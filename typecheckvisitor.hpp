#ifndef	TYPECHECKVISITOR
#define TYPECHECKVISITOR

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

#include "noticeexception.hpp"
#include "type.hpp"
#include "functionsymbol.hpp"

#include "functionscope.hpp"
#include "globalscope.hpp"
#include "structurescope.hpp"
#include "structuresymbol.hpp"


class TypeVisitor: public BasicVisitor{
protected:
	GlobalScope globalScope;
	AbstractScope *currentScope;
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

	TypeVisitor();
	virtual ~TypeVisitor() override;

	void setCurrentScope(AbstractScope*);
	void restoreCurrentScope();

	void dump(ostream *out);
};


#endif