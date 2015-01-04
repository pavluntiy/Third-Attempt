#ifndef DOTNODE
#define DOTNODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "header.hpp"
#include "token.hpp"

class BasicSymbol;
class DotNode: public BasicNode {
protected:
	string name;
	BasicNode *left;
	BasicNode *right;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;


	DotNode();
	DotNode(Position position);
	DotNode(Token token);

	BasicNode* getLeft();
	BasicNode* getRight();

	void setLeft(BasicNode *left);
	void setRight(BasicNode *right);

};

#endif