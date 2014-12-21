#ifndef PARSER
#define PARSER

#include "token.h"
#include "node.h"
#include "parser_exception.h"
#include <vector>
#include <set>
#include <stack>
#include <map>


class Parser{

protected: 
	Node *tree;
	//vector<Token> &tokens;
	Lexer &lexer;

	stack<int> history;
	map<pair<Node::Type, int>, pair<bool, int> > memo;
	int currentPosition;
	Token currentToken;



public:
	Parser (Lexer &lexer):
		lexer(lexer)
	{
		this->currentPosition = 0;
		this->currentToken = this->lexer[0];
	}


void dfs (Node *node, ostream *treeOut, string shift = ""){
	*treeOut << shift << "( " << node->typeToText();
	if(node->getText() != ""){
	 	*treeOut<< " \"" << node->getText() << "\"";
	}
	if(node->getChildren().size() > 0){
	 	*treeOut << ", children num = "<< node->getChildren().size() << ":\n";
	}
		for(auto it : node->getChildren()){
			dfs(it, treeOut, shift + ' ');
		}
	if(node->getChildren().size() > 0){
	 	*treeOut << shift;
	}
	else {
		*treeOut << ' ';
	}
	*treeOut   << ")\n";
	
}


void consume(){
	++this->currentPosition;
	if(this->lexer.isValidIndex(this->currentPosition)){	
		this->currentToken = this->lexer[this->currentPosition];
		while(this->lexer.isValidIndex(this->currentPosition) && 
				this->currentToken.isInvisible()
			)
		{
			++this->currentPosition;
			this->currentToken = this->lexer[this->currentPosition];
		}	
	}
//	else{
//	//	throw ParserException("Structure of input corrupted!");
//	}
}

Node *getBEGIN(){
	if(currentToken.typeEqualsTo(Token::BEGIN)){
		consume();
		return new Node(Node::BEGIN);
	}
	else {
		throw ParserException("No BEGIN found!");
	}
}

Node *getEND(){
	if(currentToken.typeEqualsTo(Token::END)){
		consume();
		return new Node(Node::END);
	}
	else {
		cout << currentToken;
		throw ParserException("No END found!");
	}
}

Node *getINT(){
	if(currentToken.typeEqualsTo(Token::INT)){
		consume();
		return new Node(Node::INT, currentToken.getText());
	}
	else {
		throw NoticeException("No INT found!");
	}
}


Node *getFLOAT(){
	if(currentToken.typeEqualsTo(Token::FLOAT)){
		consume();
		return new Node(Node::FLOAT, currentToken.getText());
	}
	else {
		throw NoticeException("No FLOAT found!");
	}
}

Node *getCHAR(){

	if(currentToken.typeEqualsTo(Token::CHAR)){
		consume();
		return new Node(Node::CHAR, currentToken.getText());
	}
	else {
		throw NoticeException("No CHAR found!");
	}
}


Node *getVALUE(){
//	Node *result = nullptr;

	try {
		return getINT();
	}
	catch (NoticeException ne){

	}

	try {
		return getFLOAT();
	}
	catch (NoticeException ne){

	}

	try {
		return getCHAR();
	}
	catch (NoticeException ne){

	}

	throw NoticeException("No VALUE found!");
}

Node *getEXPR8(){

	try {
		return getVALUE();
	}
	catch (NoticeException ne){

	}

	throw NoticeException("No getEXPR8 found!");
}

Node *getEXPRESSION(){
	try {
		return getEXPR8();
	}
	catch (NoticeException ne){

	}

	throw NoticeException("No EXPRESSION found!");	
}

void printTree(Node *tree, ostream *out){
	dfs(tree, out);
}
	
Node *getTree(){
	return this->tree;
}

void buildTree(){
	this->tree = new Node(Node::PROGRAM);
	this->tree->addChild(getBEGIN());
	this->tree->addChild(getEXPRESSION());
	this->tree->addChild(getEND());
}

};


#endif