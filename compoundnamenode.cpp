#include "compoundnamenode.hpp"

CompoundNameNode::CompoundNameNode(){}
CompoundNameNode::CompoundNameNode(Position position):BasicNode(position)
{

}

void CompoundNameNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}


	string CompoundNameNode::transformOperators(const Token &token){
		auto text = token.getText();
		return text;
	}


	string CompoundNameNode::toString(){
		return "COMPOUND_NAME";
	}

	vector<string>& CompoundNameNode::getNames(){
		return this->names;
	}

	void CompoundNameNode::addName(const Token &token){
		this->names.push_back(transformOperators(token));
	}

	void CompoundNameNode::addName(string what){
		this->names.push_back(what);
	}

	CompoundNameNode::CompoundNameNode(const Token &token){
		this->names.push_back(transformOperators(token));
	}

	CompoundNameNode::CompoundNameNode(string what){
		this->names.push_back(what);
	}

	bool CompoundNameNode::isSimpleName(){
		return this->names.size() == 1;
	}

	string CompoundNameNode::getName(){
		string result = "";
		for(auto it: this->names){
			result += "." + it;
		}
		return result;
	}

	string CompoundNameNode::getSimpleName(){
		return *(end(this->names) - 1);
	}

	// BasicSymbol* CompoundNameNode::getSymbol(){
	// 	return this->symbol;
	// }

	// void CompoundNameNode::setSymbol(BasicSymbol *symbol){
	// 	this->symbol = symbol;
	// }