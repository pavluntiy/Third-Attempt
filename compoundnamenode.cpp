#include "compoundnamenode.hpp"

void CompoundNameNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
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

	// string CompoundNameNode::getLeft(){
	// 	return this->left;
	// }
	
	// CompoundNameNode* CompoundNameNode::getRight(){
	// 	return this->right;
	// }

	CompoundNameNode::CompoundNameNode(){}

	CompoundNameNode::CompoundNameNode(string what){
		this->names.push_back(what);
	}