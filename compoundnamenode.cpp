#include "compoundnamenode.hpp"

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

	void CompoundNameNode::addName(string what){
		this->names.push_back(what);
	}

	CompoundNameNode::CompoundNameNode(const Token &token){
		this->names.push_back(transformOperators(token));
	}

	CompoundNameNode::CompoundNameNode(){}

	CompoundNameNode::CompoundNameNode(string what){
		this->names.push_back(what);
	}