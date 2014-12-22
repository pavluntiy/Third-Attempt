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
				(
					this->currentToken.isInvisible() 
					||
					this->currentToken.typeEqualsTo(Token::NONE)
					||
					this->currentToken.typeEqualsTo(Token::ERROR)

				)
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
		throw ParserException("No END found, got " + currentToken.toString() + " instead");
	}
}

Node *getINT(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::INT)){
		result = new Node(Node::INT, currentToken.getText());
		consume();
		return result;
	}
	else {
		throw NoticeException("No INT found!");
	}
}


Node *getFLOAT(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::FLOAT)){
		result = new Node(Node::FLOAT, currentToken.getText());
		consume();
		return result;
	}
	else {
		throw NoticeException("No FLOAT found!");
	}
}

Node *getCHAR(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::CHAR)){
		result = new Node(Node::CHAR, currentToken.getText());
		consume();
		return result;
	}
	else {
		throw NoticeException("No CHAR found!");
	}
}

Node *getSTRING(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::STRING)){
		result = new Node(Node::STRING, currentToken.getText());
		consume();
		return result;
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

Node *getATOM(){
	try{
		return getNAME();
	}
	catch(NoticeException ne){

	}
	//	cout << "YEAH!\n";
	try{
		return getVALUE();
	}
	catch(NoticeException ne){

	}

	throw NoticeException("No ATOM found at " + currentToken.getPosition().toString());

}

Node *getNAME(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::NAME)){
		result = new Node(Node::NAME, currentToken.getText());
		consume();
		return result;
	}
	throw NoticeException("No NAME");
}

Node *getFUNCARGS(){
	try {
		return getEXPRESSION();
	}
	catch (NoticeException ne){

	}
	//	cout << "asdf";
	throw NoticeException("No FUNCARGS found!");
}

Node *getFUNCCALS(Node *left){

		Node *tmp = nullptr, *right = nullptr;
		if(currentToken.typeEqualsTo(Token::BRACE_LEFT)){
					lock();
					consume();
					right = getFUNCARGS();
					if(!currentToken.typeEqualsTo(Token::BRACE_RIGHT)){
						throw ParserException("Missed BRACE_RIGHT at " + currentToken.getPosition().toString());
					}
					consume();
					tmp = new Node(Node::FUNCCALL);
					tmp->addChild(left);
					tmp->addChild(right);

					try {
						return getFUNCCALS(tmp);
					}
					catch(NoticeException ne){

					}
		}
	
		return left;
}

Node *getACCESSARGS(){
	try {
		return getEXPRESSION();
	}
	catch (NoticeException ne){

	}
//	cout << "asdf";
	throw NoticeException("No FUNCARGS found!");
}

Node *getACCESSES(Node *left){

		Node *tmp = nullptr, *right = nullptr;
		if(currentToken.typeEqualsTo(Token::BRACKET_LEFT)){
					lock();
					consume();
					right = getACCESSARGS();
					if(!currentToken.typeEqualsTo(Token::BRACKET_RIGHT)){
						throw ParserException("Missed BRACKET_RIGHT at " + currentToken.getPosition().toString());
					}
					consume();
					tmp = new Node(Node::ACCESS);
					tmp->addChild(left);
					tmp->addChild(right);

					try {
						return getACCESSES(tmp);
					}
					catch(NoticeException ne){

					}
		}
	
		return left;
}

Node* getBRACED(Node *left = nullptr){
	//cout << "mimimi\n";
	if(left == nullptr){
		throw ParserException("Unexpected BRACE_LEFT or BRACKET_RIGHT at " + currentToken.getPosition().toString());
	}

	while(currentToken.typeEqualsTo(Token::BRACE_LEFT) || currentToken.typeEqualsTo(Token::BRACKET_LEFT)){
		if(currentToken.typeEqualsTo(Token::BRACE_LEFT)){
			left = getFUNCCALS(left);
		}
		else {
			left = getACCESSES(left);
		}
	}

	return left;
}

Node *getEXPR9_OP_SUFFIX(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(currentToken.getText() == "--"){
			result = new Node(Node::OPERATOR, "_--");
			consume();
		//	cout << currentToken.getText();
			return result;
		}

		if(currentToken.getText() == "++"){
			result = new Node(Node::OPERATOR, "_++");
			consume();
			return result;
		}
	}

	throw NoticeException("No EXPR10_OP_SUFFIX found!");
}

Node *getEXPR9_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(currentToken.getText() == "--"){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
		//	cout << currentToken.getText();
			return result;
		}

		if(currentToken.getText() == "++"){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

		if(currentToken.getText() == "$"){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

		if(currentToken.getText() == "@"){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

		if(currentToken.getText() == "+"){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

		if(currentToken.getText() == "-"){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

		if(currentToken.getText() == "~"){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

		if(currentToken.getText() == "!"){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}
	}

	throw NoticeException("No EXPR9_OP found!");
}

Node *getEXPR9_SUFFIX(Node *left = nullptr){
	Node *op = nullptr;
	Node *tmp = nullptr;

	if(left == nullptr){
		try{
			left = getEXPR10();
		}
		catch (ParserException pe){
			throw ParserException("Strange stuff at " + currentToken.getPosition().toString() + ";\n got " + currentToken.toString());
		}
	}

	try{
		op = getEXPR9_OP_SUFFIX();
		tmp = getEXPR9_SUFFIX(left);
		op->addChild(tmp);
		return op;
	}
	catch(NoticeException ne){
	}

	return left;
}

Node *getEXPR9(){
	Node *son= nullptr;
	Node *op = nullptr;
	try {
		op = getEXPR9_OP();
		son = getEXPR9();
		op->addChild(son);
	}
	catch (NoticeException ne){

	}

	try {
		return getEXPR9_SUFFIX();
	}
	catch (NoticeException ne){

	}

	if(op == nullptr){
		throw NoticeException("No EXPR10 found!");
	}

	return op;
}

Node *getEXPR10(Node *left = nullptr){
	lock();
	//	Node *tmp = nullptr;
	Node *right = nullptr;
	Node *op = nullptr;
	if(left == nullptr){
	//	cout << "ololo!";
		try{
			left = getATOM();
			left = getBRACED(left);
			//cout << "Nya!\n";
			try{
				op = getEXPR10_OP();
				right = getATOM();
				op->addChild(left);
				op->addChild(right);
				return getEXPR10(op);
			}
			catch(NoticeException ne){
			}

			return left;
		}
		catch(NoticeException ne){

		}
	}
	else {	
			left = getBRACED(left);
			try{
				op = getEXPR10_OP();
				right = getATOM();
				op->addChild(left);
				op->addChild(right);
				return getEXPR10(op);
			}
			catch(NoticeException ne){
				return getBRACED(left);
			}
			return left;
	}

	try {
		return getATOM();
	}
	catch(NoticeException ne){

	}

	throw NoticeException("No EXPR10 found!");
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

	try {
		return getSTRING();
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
		return getEXPR9();
	}
	catch (NoticeException ne){

	}
	//for empty one
	return new Node(Node::EXPRESSION);
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
	try {
		this->tree->addChild(getEXPRESSION());
	}
	catch(NoticeException ne){

	}
	this->tree->addChild(getEND());
}

};


#endif