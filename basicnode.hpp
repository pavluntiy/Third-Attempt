#ifndef BASICNODE
#define BASICNODE
#include "header.hpp"
#include "basicvisitor.hpp"

class BasicSymbol;

class BasicNode {
protected:
	string text;
	Position position;
	BasicSymbol *symbol;
public:
	virtual void accept(BasicVisitor *visitor) = 0;
	virtual string toString() = 0;
	virtual void addChild(BasicNode *){}
	virtual string getText();

	virtual ~BasicNode();

	Position getPosition();
	BasicSymbol *getSymbol();

};

#endif