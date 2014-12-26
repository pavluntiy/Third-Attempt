#ifndef BASICVISITOR
#define BASICVISITOR
#include "header.hpp"
class ProgramNode;
class ExpressionNode;
class TypeNode;
class FunctionCallNode;
class CompoundNameNode;
class ValueNode;


class BasicVisitor {

public:
	virtual void visit(ProgramNode *node) = 0;
	virtual void visit(ExpressionNode *node) = 0;
	// virtual void visit(TypeNode *node) = 0;
	//virtual void visit(FunctionCallNode *node) = 0;
	// virtual void visit(CompoundNameNode *node) = 0;
	virtual void visit(ValueNode *node) = 0;

};	

#endif