#include "valuenode.hpp"

ValueNode::ValueNode(){}
ValueNode::ValueNode(Position position):BasicNode(position)
{

}


	void ValueNode::accept(BasicVisitor *visitor){
		visitor->visit(this);
	}

	string ValueNode::toString(){
		return this->getTypeText();
	}

	string ValueNode::getTypeText(){
		switch(this->type){
			case Type::INT: return "INT";
			case Type::FLOAT: return "FLOAT";
			case Type::STRING: return "STRING";
			case Type::CHAR: return "CHAR";
			default: return "NONE";
		}
	}

	ValueNode::Type ValueNode::getType(){
		return this->type;
	}

	ValueNode::ValueNode(Token::Type type, string text){
		switch (type){
			case Token::INT: this->type = Type::INT; break;
			case Token::FLOAT: this->type = Type::FLOAT; break;
			case Token::CHAR: this->type = Type::CHAR; break;
			case Token::STRING: this->type = Type::STRING; break;
			default: return;
		}

		this->text = text;
	}

