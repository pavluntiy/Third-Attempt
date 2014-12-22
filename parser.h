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


void lock(){
	this->history.push(currentPosition);
}
void recoil(){
	if(this->history.empty()){
		throw ParserException("Incorrect recoiling!");
	}

	this->currentPosition = this->history.top();
	this->history.pop();
	this->currentToken = this->lexer[this->currentPosition];
}
void memoize(Node::Type type, int position, bool success, int jump){
	this->memo[make_pair(type, position)] = make_pair(success, jump);
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
	//	cout << currentToken;
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

Node *getEXPR10_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(currentToken.getText() == "."){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
		//	cout << currentToken.getText();
			return result;
		}

		if(currentToken.getText() == "->"){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}
	}

	throw NoticeException("No EXPR10_OP found!");
}

Node *getNAME(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::NAME)){
		result = new Node(Node::NAME, currentToken.getText());
		consume();
		return result;
	}
	throw ParserException("No name found but grammar requires at " + currentToken.getPosition().toString());
}

Node *getEXPR10(Node *parent = nullptr){
	lock();
	Node *tmp = nullptr;
	Node *left = nullptr, *right = nullptr;
	Node *op = nullptr;
	if(parent == nullptr){
	//	cout << "ololo!";
		if(currentToken.typeEqualsTo(Token::NAME)){
			left = getNAME();
			try{
				op = getEXPR10_OP();
				right = getNAME();
				op->addChild(left);
				op->addChild(right);
				return getEXPR10(op);
			}
			catch(NoticeException ne){
			}
		//	cout << "ololo!";
			return left;
		}
	}
	else {
			try{
				op = getEXPR10_OP();
				right = getNAME();
				op->addChild(parent);
				op->addChild(right);
				return getEXPR10(op);
			}
			catch(NoticeException ne){
			}
			return parent;
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
	lock();
	try {
		return getVALUE();
	}
	catch (NoticeException ne){

	}

	throw NoticeException("No getEXPR8 found!");
}

Node *getEXPRESSION(){
	lock();
	try {
		return getEXPR10();
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