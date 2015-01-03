#ifndef BASICNODE
#define BASICNODE
#include "header.hpp"
#include "basicvisitor.hpp"


class AbstractScope;
class BasicSymbol;
class BasicNode {
protected:
	string text;
	Position position;
	AbstractScope *scope;
	BasicSymbol *symbol;

public:
	virtual void accept(BasicVisitor *visitor) = 0;
	virtual string toString() = 0;
	virtual void addChild(BasicNode *){}
	virtual string getText();

	virtual ~BasicNode();

	BasicNode();
	BasicNode(Position position);

	Position getPosition();
	void setPosition(Position position);

	AbstractScope* getScope();
	void setScope(AbstractScope *scope);

	BasicSymbol *getSymbol();
	void setSymbol(BasicSymbol *symbol);

};

#endif