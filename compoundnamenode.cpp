#include "compoundnamenode.hpp"

void CompoundNameNode::accept(BasicVisitor *visitor){
	visitor->visit(this);
}
	string CompoundNameNode::toString(){
		return "COMPOUND_NAME";
	}

	string CompoundNameNode::getLeft(){
		return this->left;
	}
	
	CompoundNameNode* CompoundNameNode::getRight(){
		return this->right;
	}


	CompoundNameNode::CompoundNameNode(string left, CompoundNameNode *right){
		this->left = left;
		this->right = right;
		//cout << "Nice!";
	}