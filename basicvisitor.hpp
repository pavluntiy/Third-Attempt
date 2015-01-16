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
class SignatureNode;
class FunctionDefinitionNode;
class IfNode;
class WhileNode;
class ForNode;
class ReturnNode;
class StructNode;
class BlockNode;
class DotNode;
class ImportNode;


class BasicVisitor {

public:
	virtual void visit(ProgramNode *node) = 0;
	virtual void visit(OperatorsNode *node) = 0;
	virtual void visit(TypeNode *node) = 0;
	virtual void visit(FunctionCallNode *node) = 0;
	virtual void visit(CompoundNameNode *node) = 0;
	virtual void visit(ValueNode *node) = 0;
	virtual void visit(VarDeclarationNode *node) = 0;
	virtual void visit(SignatureNode *node) = 0;
	virtual void visit(FunctionDefinitionNode *node) = 0;
	virtual void visit(IfNode *node) = 0;
	virtual void visit(WhileNode *node) = 0;
	virtual void visit(ForNode *node) = 0;
	virtual void visit(ReturnNode *node) = 0;
	virtual void visit(StructNode *node) = 0;
	virtual void visit(BlockNode *node) = 0;
	virtual void visit(DotNode *node) = 0;
	virtual void visit(ImportNode *node) = 0;

	virtual ~BasicVisitor();

};	

#endif