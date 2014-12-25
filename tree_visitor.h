#ifndef TREE_VISITOR
#define TREE_VISITOR

#include <iostream>
#include <vector>
#include "node.hpp"


class TreeVisitor{

	ostream *out;
	Node *tree;
public:

	TreeVisitor(ostream *out):out(out){

	}

	void dfs (Node *node, string shift = ""){
	*this->out << shift << "( " << node->typeToText();
	if(node->getText() != ""){
	 	*this->out<< " \"" << node->getText() << "\"";
	}
	if(node->getChildren().size() > 0){
	 	*this->out << ", children num = "<< node->getChildren().size() << ":" << endl;
	}
		for(auto it : node->getChildren()){
			dfs(it, shift + ' ');
		}
	if(node->getChildren().size() > 0){
	 	*this->out << shift;
	}
	else {
		*this->out << ' ';
	}
	*this->out   << ")" << endl;
	
}


void printTree(){
	dfs(this->tree);
}
void deleteTree(){
	deleteTree(this->tree);
}

Node *clone(Node *tree){
	if(tree == nullptr){
		return nullptr;
	}

	Node *result = new Node(*tree);

	vector<Node*> &children = tree->getChildren();
	for(int i = 0; i < (int) children.size(); ++i){
		result->addChild(clone(children[i]));
	}
	return result;
}

void deleteTree(Node *tree){
	if(tree == nullptr){
		return;
	}

	vector<Node*> &children = tree->getChildren();
	for(int i = 0; i < (int) children.size(); ++i){
		deleteTree(children[i]);
	}
	delete tree;
}
// void visitTree(ostream *this->out){
// 	tree = new Node(Node::PROGRAM);
// 	tree->addChild(getBEGIN());
// 	visitTree(tree, this->out);
// }

// void visitTree (Node *node, ostream *this->out, string shift = ""){
// 	*this->out << shift << "( " << node->typeToText();
// 	if(node->getText() != ""){
// 	 	*this->out<< " \"" << node->getText() << "\"";
// 	}

// 	int counter = 0;
// 	// if(node->getChildren().size() > 0){
// 	//  	*this->out << ", children num = "<< node->getChildren().size() << ":\n";
// 	// }

// 	// for(auto it : node->getChildren()){
// 	// 	dfs(it, this->out, shift + ' ');
// 	// }

// 	try {
// 		dfs(getEXPRESSION(), this->out, shift + ' ');
// 	}
// 	catch (NoticeException ne){

// 	}


// 	if(counter > 0){
// 	 	*this->out << shift;
// 	}
// 	else {
// 		*this->out << ' ';
// 	}
// 	*this->out   << ")\n";
	
// }

void setTree(Node *tree){
	this->tree = tree;
}

};

#endif
