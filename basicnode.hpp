#ifndef BASICNODE
#define BASICNODE
#include "header.hpp"
#include "basicvisitor.hpp"

class BasicNode {
friend class BasicVisitor;
protected:
	string text;
public:
	virtual void accept(BasicVisitor *visitor) = 0;
	virtual string toString() = 0;
	virtual void addChild(BasicNode *){}
	virtual string getText();

};

#endif