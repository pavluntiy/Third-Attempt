#ifndef BASICNODE
#define BASICNODE
#include "header.hpp"
#include "basicvisitor.hpp"


class BasicNode {
protected:
	string text;
	Position position;
public:
	virtual void accept(BasicVisitor *visitor) = 0;
	virtual string toString() = 0;
	virtual void addChild(BasicNode *){}
	virtual string getText();

	virtual ~BasicNode();

	BasicNode();
	BasicNode(Position position);

	Position getPosition();

};

#endif