#ifndef VALUENODE
#define VALUENODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "token.hpp"

class ValueNode: public BasicNode {

friend class BasicVisitor;


public:
	enum class Type {
		INT,
		FLOAT,
		CHAR,
		STRING
	};
protected:
	Type type;
	string value;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;
	string getType();

	ValueNode(Token::Type type, string text);


};

#endif