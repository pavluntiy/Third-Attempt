#ifndef	TYPECHECKVISITOR
#define TYPECHECKVISITOR

#include "basicvisitor.hpp"
#include "parser.hpp"

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
#include "usingnode.hpp"

#include "noticeexception.hpp"
#include "parserexception.hpp"

#include "type.hpp"
#include "arraytype.hpp"
#include "functiontype.hpp"


#include "functionsymbol.hpp"
#include "functioncallsymbol.hpp"
#include "valuesymbol.hpp"
#include "importsymbol.hpp"

#include "functionscope.hpp"
#include "globalscope.hpp"
#include "structurescope.hpp"
#include "localscope.hpp"
#include "structuresymbol.hpp"
#include "variablesymbol.hpp"


class TypeVisitor: public BasicVisitor{
protected:
	AbstractScope *globalScope;
	AbstractScope *currentScope;
	stack<AbstractScope*> scopes;
	bool globalScopeFound;
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


	BasicNode* insertConversion(BasicNode*, FunctionSymbol*);
	BasicNode* import(AbstractScope *to, string name);

	TypeVisitor();
	TypeVisitor(AbstractScope* scope);
	virtual ~TypeVisitor() override;

	void setCurrentScope(AbstractScope*);
	void restoreCurrentScope();

	void dump(ostream *out);

	FunctionCallSymbol* setFunctionName(FunctionCallSymbol*);

	AbstractScope *getGlobalScope();
	void setGlobalScope(AbstractScope*);
};


#endif