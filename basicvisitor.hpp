#ifndef BASICVISITOR
#define BASICVISITOR

#include "header.hpp"

class ProgramNode;
class OperatorsNode;
class TypeNode;
class FunctionCallNode;
class CompoundNameNode;
class ValueNode;
class VarDeclarationNode;


class BasicVisitor {

public:
	virtual void visit(ProgramNode *node) = 0;
	virtual void visit(OperatorsNode *node) = 0;
	virtual void visit(TypeNode *node) = 0;
	virtual void visit(FunctionCallNode *node) = 0;
	virtual void visit(CompoundNameNode *node) = 0;
	virtual void visit(ValueNode *node) = 0;
	virtual void visit(VarDeclarationNode *node) = 0;

};	

#endif