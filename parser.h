#ifndef PARSER
#define PARSER

#include "token.h"
#include "node.h"
#include "parser_exception.h"
#include "tree_visitor.h"
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
	TreeVisitor &visitor;



public:
	Parser (Lexer &lexer, TreeVisitor &visitor):
		lexer(lexer), visitor(visitor)
	{
		this->currentPosition = 0;
		this->currentToken = this->lexer[0];
		this->history.push(currentPosition);
	}

void lock(){
	this->history.push(currentPosition);
}
void recoil(){

//	cout << "recoiling!!!!!!!!!!!!!!!!!!!!!\n";
	if(this->history.empty()){
		throw ParserException("Incorrect recoiling!");
	}

	this->currentPosition = this->history.top();
	this->history.pop();
	this->currentToken = this->lexer[this->currentPosition];
//	cout << currentToken << '\n';
}

void memoize(Node::Type type, int position, bool success, int jump){
	this->memo[make_pair(type, position)] = make_pair(success, jump);
}


void consume(){
	++this->currentPosition;
	if(this->lexer.isValidIndex(this->currentPosition)){	
		this->currentToken = this->lexer[this->currentPosition];
		while(currentToken.isIgnorable())
		{
			++this->currentPosition;
			this->currentToken = this->lexer[this->currentPosition];

		}	
	}
//	else{
//	//	throw ParserException("Structure of input corrupted!");
//	}
}

void get(Token token){
	if(! (currentToken == token) ){
		throw NoticeException ("No " + token.toString() + " found, got " + currentToken.toString() + 
			"instead at " + currentToken.getPosition().toString());
	}
	consume();
}

void get(Token::Type type){
	if(! currentToken.typeEqualsTo(type) ){
		throw NoticeException ("No " + Token(type).toString() + " found, got " + currentToken.toString() + 
			"instead at " + currentToken.getPosition().toString());
	}
	consume();
}

Node *getCOMMA(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(
			currentToken.getText() == ","
		){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

	}

	throw NoticeException("No COMMA found!");
}

Node *getCOMMA_EXPRESSION(Node *left = nullptr){

		Node *op = nullptr, *right = nullptr;
		if(left == nullptr){
			try{
				left = getASSIGNMENT();
			}
			catch (NoticeException ne){
				throw NoticeException("No COMMA_EXPRESSION found!");
			}
		}

		try {
			op = getCOMMA();
		}
		catch (NoticeException ne){
			return left;
		}

		try {
			right = getASSIGNMENT();
			op->addChild(left);
			op->addChild(right);
			return getCOMMA_EXPRESSION(op);
		}
		catch (NoticeException ne){
			throw ParserException("Operator for COMMA_EXPRESSION without operands at " + currentToken.getPosition().toString());
		}

		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

}

Node *getASSIGNMENT_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(
			currentToken.getText() == "="
			||
			currentToken.getText() == "*="
			||
			currentToken.getText() == "/="
			||
			currentToken.getText() == "%="
			||
			currentToken.getText() == "+="
			||
			currentToken.getText() == "-="
			||
			currentToken.getText() == ">>="
			||
			currentToken.getText() == ">>>="
			||
			currentToken.getText() == "<<="
			||
			currentToken.getText() == "&="
			||
			currentToken.getText() == "^="
			||
			currentToken.getText() == "|="
			||
			currentToken.getText() == "&&="
			||
			currentToken.getText() == "||="
			||
			currentToken.getText() == ":="
		){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

	}

	throw NoticeException("No EXPR8_OP found!");
}

Node *getASSIGNMENT(){
	Node *left = nullptr;
	Node *op = nullptr;
	Node *tmp = nullptr;

	try{
		left = getTERNARY();
	}
	catch (NoticeException ne){
	}

	try{
		op = getASSIGNMENT_OP();
		op->addChild(left);
		tmp = getASSIGNMENT();
		op->addChild(tmp);
		return op;
	}
	catch(NoticeException ne){
	}

	if(left == nullptr){
		throw NoticeException("No ASSIGNMENT found!");
	}
	return left;
}

Node *getTERNARY(){
	Node *op = nullptr;
	Node *cond = nullptr, *first = nullptr, *second = nullptr;

	try {
		cond = getL_AND();
	}
	catch (NoticeException ne){}

	if(cond == nullptr){
		throw NoticeException("No TERNARY found!");
	}


	if(currentToken.typeEqualsTo(Token::OPERATOR) && currentToken.getText() == "?"){
		consume();
		op = new Node(Node::OPERATOR, "?");
	}
	else {
		return cond;
	}

	first = getTERNARY();

	if(currentToken.typeEqualsTo(Token::OPERATOR) && currentToken.getText() == ":"){
		consume();
	}
	else{
		throw ParserException("Unfinished TERNARY operator at " + currentToken.getPosition().toString());
	}

	second = getTERNARY();

	op->addChild(cond);
	op->addChild(first);
	op->addChild(second);


	
	return op;
}

Node *getIS_IN_EXPRESSION_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::KEYWORD)){
		if(
			currentToken.getText() == "in"
			||
			currentToken.getText() == "is"
		){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

	}

	throw NoticeException("No IS_IN_EXPRESSION_OP found!");
}

Node *getIS_IN_EXPRESSION(Node *left = nullptr){

		Node *op = nullptr, *right = nullptr;
		if(left == nullptr){
			try{
				left = getSHIFT();
			}
			catch (NoticeException ne){
				throw NoticeException("No IS_IN_EXPRESSION found!");
			}
		}

		try {
			op = getIS_IN_EXPRESSION_OP();
		}
		catch (NoticeException ne){
			return left;
		}

		try {
			right = getSHIFT();
			op->addChild(left);
			op->addChild(right);
			return getIS_IN_EXPRESSION(op);
		}
		catch (NoticeException ne){
			throw ParserException("Operator for IS_IN_EXPRESSION without operands at " + currentToken.getPosition().toString());
		}

		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

}


Node *getL_OR_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(
			currentToken.getText() == "||"
		){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

	}

	throw NoticeException("No L_OR_OP found!");
}

Node *getL_OR(Node *left = nullptr){

		Node *op = nullptr, *right = nullptr;
		if(left == nullptr){
			try{
				left = getL_AND();
			}
			catch (NoticeException ne){
				throw NoticeException("No L_OR found!");
			}
		}

		try {
			op = getL_OR_OP();
		}
		catch (NoticeException ne){
			return left;
		}

		try {
			right = getL_AND();
			op->addChild(left);
			op->addChild(right);
			return getL_OR(op);
		}
		catch (NoticeException ne){
			throw ParserException("Operator for L_AND without operands at " + currentToken.getPosition().toString());
		}

		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

}


Node *getL_AND_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(
			currentToken.getText() == "&&"
		){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

	}

	throw NoticeException("No L_AND_OP found!");
}

Node *getL_AND(Node *left = nullptr){

		Node *op = nullptr, *right = nullptr;
		if(left == nullptr){
			try{
				left = getB_OR();
			}
			catch (NoticeException ne){
				throw NoticeException("No L_AND found!");
			}
		}

		try {
			op = getL_AND_OP();
		}
		catch (NoticeException ne){
			return left;
		}

		try {
			right = getB_OR();
			op->addChild(left);
			op->addChild(right);
			return getL_AND(op);
		}
		catch (NoticeException ne){
			throw ParserException("Operator for L_AND without operands at " + currentToken.getPosition().toString());
		}

		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

}

Node *getCOMPARISION_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(
			currentToken.getText() == "=="
			||
			currentToken.getText() == "!="
		){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

	}

	throw NoticeException("No COMPARISION_OP found!");
}
Node *getCOMPARISION(Node *left = nullptr){

		Node *op = nullptr, *right = nullptr;
		if(left == nullptr){
			try{
				left = getDIFF_COMPARISION();
			}
			catch (NoticeException ne){
				throw NoticeException("No COMPARISION found!");
			}
		}

		try {
			op = getCOMPARISION_OP();
		}
		catch (NoticeException ne){
			return left;
		}

		try {
			right = getDIFF_COMPARISION();
			op->addChild(left);
			op->addChild(right);
			return getCOMPARISION(op);
		}
		catch (NoticeException ne){
			throw ParserException("Operator for COMPARISION without operands at " + currentToken.getPosition().toString());
		}

		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

}

Node *getDIFF_COMPARISION_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(
			currentToken.getText() == "<="
			||
			currentToken.getText() == ">="
			||
			currentToken.getText() == "<"
			||
			currentToken.getText() == ">"
		){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

	}

	throw NoticeException("No DIFF_COMPARISION_OP found!");
}
Node *getDIFF_COMPARISION(Node *left = nullptr){

		Node *op = nullptr, *right = nullptr;
		if(left == nullptr){
			try{
				left = getIS_IN_EXPRESSION();
			}
			catch (NoticeException ne){
				throw NoticeException("No DIFF_COMPARISION found!");
			}
		}

		try {
			op = getDIFF_COMPARISION_OP();
		}
		catch (NoticeException ne){
			return left;
		}

		try {
			right = getIS_IN_EXPRESSION();
			op->addChild(left);
			op->addChild(right);
			return getDIFF_COMPARISION(op);
		}
		catch (NoticeException ne){
			throw ParserException("Operator for DIFF_COMPARISION without operands at " + currentToken.getPosition().toString());
		}

		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

}

Node *getB_OR_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(
			currentToken.getText() == "|"
		){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

	}

	throw NoticeException("No B_OR_OP found!");
}
Node *getB_OR(Node *left = nullptr){

		Node *op = nullptr, *right = nullptr;
		if(left == nullptr){
			try{
				left = getB_XOR();
			}
			catch (NoticeException ne){
				throw NoticeException("No B_XOR found!");
			}
		}

		try {
			op = getB_OR_OP();
		}
		catch (NoticeException ne){
			return left;
		}

		try {
			right = getB_XOR();
			op->addChild(left);
			op->addChild(right);
			return getB_OR(op);
		}
		catch (NoticeException ne){
			throw ParserException("Operator for B_OR without operands at " + currentToken.getPosition().toString());
		}

		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

}

Node *getB_XOR_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(
			currentToken.getText() == "^"
		){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

	}

	throw NoticeException("No B_XOR_OP found!");
}
Node *getB_XOR(Node *left = nullptr){

		Node *op = nullptr, *right = nullptr;
		if(left == nullptr){
			try{
				left = getB_AND();
			}
			catch (NoticeException ne){
				throw NoticeException("No B_XOR found!");
			}
		}

		try {
			op = getB_XOR_OP();
		}
		catch (NoticeException ne){
			return left;
		}

		try {
			right = getB_AND();
			op->addChild(left);
			op->addChild(right);
			return getB_XOR(op);
		}
		catch (NoticeException ne){
			throw ParserException("Operator for B_XOR without operands at " + currentToken.getPosition().toString());
		}

		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

}

Node *getB_AND_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(
			currentToken.getText() == "&"
		){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

	}

	throw NoticeException("No B_AND_OP found!");
}
Node *getB_AND(Node *left = nullptr){

		Node *op = nullptr, *right = nullptr;
		if(left == nullptr){
			try{
				left = getCOMPARISION();
			}
			catch (NoticeException ne){
				throw NoticeException("No B_AND found!");
			}
		}

		try {
			op = getB_AND_OP();
		}
		catch (NoticeException ne){
			return left;
		}

		try {
			right = getCOMPARISION();
			op->addChild(left);
			op->addChild(right);
			return getB_AND(op);
		}
		catch (NoticeException ne){
			throw ParserException("Operator for B_AND without operands at " + currentToken.getPosition().toString());
		}

		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

}



Node *getSHIFT_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(
			currentToken.getText() == "<<"
			||
			currentToken.getText() == ">>"
			||
			currentToken.getText() == ">>>"
		){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

	}

	throw NoticeException("No SHIFT_OP found!");
}
Node *getSHIFT(Node *left = nullptr){

		Node *op = nullptr, *right = nullptr;
		if(left == nullptr){
			try{
				left = getEXPR6();
			}
			catch (NoticeException ne){
				throw NoticeException("No EXPR6 found!");
			}
		}

		try {
			op = getSHIFT_OP();
		}
		catch (NoticeException ne){
			return left;
		}

		try {
			right = getEXPR6();
			op->addChild(left);
			op->addChild(right);
			return getSHIFT(op);
		}
		catch (NoticeException ne){
			throw ParserException("Operator for SHIFT without operands at " + currentToken.getPosition().toString());
		}

		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

}

Node *getEXPR6_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(
			currentToken.getText() == "+"
			||
			currentToken.getText() == "-"
		){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

	}

	throw NoticeException("No EXPR6_OP found!");
}
Node *getEXPR6(Node *left = nullptr){

		Node *op = nullptr, *right = nullptr;
		if(left == nullptr){
			try{
				left = getEXPR7();
			}
			catch (NoticeException ne){
				throw NoticeException("No EXPR6 found!");
			}
		}

		try {
			op = getEXPR6_OP();
		}
		catch (NoticeException ne){
			return left;
		}

		try {
			right = getEXPR7();
			op->addChild(left);
			op->addChild(right);
			return getEXPR6(op);
		}
		catch (NoticeException ne){
			throw ParserException("Operator for EXPR6 without operands at " + currentToken.getPosition().toString());
		}

		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

}

Node *getEXPR7_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(
			currentToken.getText() == "/"
			||
			currentToken.getText() == "%"
			||
			currentToken.getText() == "*"
		){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}

	}

	throw NoticeException("No EXPR7_OP found!");
}
Node *getEXPR7(Node *left = nullptr){

		Node *op = nullptr, *right = nullptr;
		if(left == nullptr){
			try{
				left = getEXPR8();
			}
			catch (NoticeException ne){
				throw NoticeException("No EXPR7 found!");
			}
		}

		try {
			op = getEXPR7_OP();
		}
		catch (NoticeException ne){
			return left;
		}

		try {
			right = getEXPR8();
			op->addChild(left);
			op->addChild(right);
			return getEXPR7(op);
		}
		catch (NoticeException ne){
			throw ParserException("Operator for EXPR7 without operands at " + currentToken.getPosition().toString());
		}

		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

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

Node *getEXPR8_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(currentToken.getText() == "**"){
			result = new Node(Node::OPERATOR, "**");
			consume();
			return result;
		}

	}

	throw NoticeException("No EXPR8_OP found!");
}

Node *getEXPR8(){
	Node *left = nullptr;
	Node *op = nullptr;
	Node *tmp = nullptr;

	try{
		left = getEXPR9();
	}
	catch (NoticeException ne){
	}

	try{
		op = getEXPR8_OP();
		op->addChild(left);
		tmp = getEXPR8();
		op->addChild(tmp);
		return op;
	}
	catch(NoticeException ne){
	}

	if(left == nullptr){
		throw NoticeException("No EXPR8 found!");
	}
	return left;
}

Node *getATOM(){
	if(currentToken.typeEqualsTo(Token::BRACE_LEFT)){
		consume();
		Node *result = getEXPRESSION();
	//	cout << "HEY!";
		if(!currentToken.typeEqualsTo(Token::BRACE_RIGHT)){
			throw ParserException("Missing BRACE_RIGHT at " + currentToken.getPosition().toString());
		}
		consume();
		return result;
	}
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
	Node  *tmp = nullptr, *result = nullptr;

	result = new Node(Node::FUNCARGS);


	try{
		tmp = getASSIGNMENT();
	}
	catch(NoticeException ne){
	//	throw NoticeException("No FUNCARGS found!");
		return result;
	};

	result->addChild(tmp);

	while(currentToken == Token(Token::OPERATOR, ",")){
		consume();
		tmp = getASSIGNMENT();
		result->addChild(tmp);
	}
	
	return result;


	//throw NoticeException("No FUNCARGS found!");
}

Node *getFUNCCALS(Node *left){

		Node *tmp = nullptr, *right = nullptr;
		if(currentToken.typeEqualsTo(Token::BRACE_LEFT)){
					lock();
					consume();
					try{
						right = getFUNCARGS();
					}
					catch (NoticeException  ne){};
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

	throw NoticeException("No EXPR9_OP_SUFFIX found , got + " + currentToken.toString());
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

	throw NoticeException("No EXPR9_OP found, got + " + currentToken.toString());
}

Node *getEXPR9_SUFFIX(Node *left = nullptr){
	Node *op = nullptr;
	Node *tmp = nullptr;
  //	cout << "AT ENTER LEFT IS " << left << '\n'; 
	if(left == nullptr){
		try{
  //			cout << "#######\n";
  //			cout << currentToken;
			left = getEXPR10();
		}
		catch (NoticeException ne){
		//	throw ParserException("Strange stuff at " + currentToken.getPosition().toString() + ";\n got " + currentToken.toString());
		}
	}

  //	cout << "ONGTWSEDFVGJS\n";
	try{
		op = getEXPR9_OP_SUFFIX();
		tmp = getEXPR9_SUFFIX(left);
		op->addChild(tmp);
		return op;
	}
	catch(NoticeException ne){
 //		cout << ne.what();
	}
 //	cout << "LEFT IS " << left << '\n';
	return left;
}

Node *getEXPR9(){
	//cout << "TEWEGSDF|\n";
	Node *son= nullptr;
	Node *op = nullptr;
	try {
		op = getEXPR9_OP();
		son = getEXPR9();
		op->addChild(son);
		return op;
	}
	catch (NoticeException ne){
		//cout << ne.what();
		//cout << "ppppp\n";
		//cout << currentToken << '\n';
	}

	try {
	//	cout << "eeeee\n";
	//	cout << currentToken;
		//cout << "currentPosition: " << currentPosition << '\n';
		return getEXPR9_SUFFIX();
	}
	catch (NoticeException ne){
		//cout << "OH YEAH!\n\n";
	}

	if(op == nullptr){
		throw NoticeException("No EXPR9 found!");
	}

	return op;
}

Node *getEXPR10(Node *left = nullptr){
	//	Node *tmp = nullptr;
	Node *right = nullptr;
	Node *op = nullptr;
	//cout << "ololo!";
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
				//visitor.deleteTree (op);
				//visitor.deleteTree(right);
				return getBRACED(left);
			}
			return left;
	}

	try {
		//cout << "OK\n";
		return getATOM();
	}
	catch(NoticeException ne){
	//	cout << ne.what();
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

Node *getNON_EMPTY_EXPRESSION(){

	lock();
	try {

		return getCOMMA_EXPRESSION();
	}
	catch (NoticeException ne){
		recoil();
	}

	throw NoticeException("No NON_EMPTY_EXPRESSION found!");
}

Node *getEXPRESSION(){

	return getNON_EMPTY_EXPRESSION();
	// lock();
	// try {

	// 	return getCOMMA_EXPRESSION();
	// }
	// catch (NoticeException ne){
	// 	recoil();
	// }
	// //for empty one
	// //return new Node(Node::EXPRESSION);

}

Node *getTYPE_MOD(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::KEYWORD)){
		if(
			currentToken.getText() == "const"
			||
			currentToken.getText() == "long"
			||
			currentToken.getText() == "short"
			||
			currentToken.getText() == "unsigned"
			||
			currentToken.getText() == "signed"
			||
			currentToken.getText() == "volatile"
			||
			currentToken.getText() == "extern"


		){
			result = new Node(Node::TYPE_MOD, currentToken.getText());
			consume();
			return result;
		}
	}

		throw NoticeException("No TYPE_MOD found!");

}

Node *getPOINTER_MOD(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(
			currentToken.getText() == "@"
		){
			result = new Node(Node::POINTER_MOD, currentToken.getText());
			consume();
			return result;
		}
	}

	if(currentToken.typeEqualsTo(Token::KEYWORD)){
		if(
			currentToken.getText() == "ref"
		){
			result = new Node(Node::POINTER_MOD, currentToken.getText());
			consume();
			return result;
		}
	}

		throw NoticeException("No POINTER_MOD found!");
}

Node *getTYPENAME_OP(){
	Node *result = nullptr;
	if(currentToken.typeEqualsTo(Token::OPERATOR)){
		if(currentToken.getText() == "."){
			result = new Node(Node::OPERATOR, currentToken.getText());
			consume();
			return result;
		}
	}

	throw NoticeException ("No '.' for COMPOUND_NAME found!");
}

Node *getCOMPOUND_NAME(Node *left = nullptr){
	Node *right = nullptr;
	Node *op = nullptr;
	if(left == nullptr){
		try{
			left = getNAME();
			try{
				op = getTYPENAME_OP();
				op->addChild(left);
				right = getNAME();
				op->addChild(right);
				return getCOMPOUND_NAME(op);
			}
			catch(NoticeException ne){
				if(op != nullptr && right == nullptr){
					visitor.deleteTree (op);
					// visitor.deleteTree(left);
					// visitor.deleteTree(right);
					throw ParserException("Strange '.' in name at " + currentToken.getPosition().toString());
				}
				return left;
			}
		}
		catch(NoticeException ne){

		}
	}
	else {	
			try{
				op = getTYPENAME_OP();
				right = getNAME();
				op->addChild(left);
				op->addChild(right);
				return getCOMPOUND_NAME(op);
			}
			catch(NoticeException ne){
				visitor.deleteTree (op);
				visitor.deleteTree(right);
			}
			return left;
	}

	try {
		return getNAME();
	}
	catch(NoticeException ne){

	}

	throw NoticeException("No COMPOUND_NAME found!");
}

Node *getTYPE(){
	Node *result = new Node(Node::TYPE);
	Node *typemods = new Node(Node::TYPE_MOD);
	Node *pointermods = new Node(Node::POINTER_MOD);

	try{
		while(1){
			typemods->addChild(getTYPE_MOD());
		}
	}
	catch (NoticeException ne){}

	result->addChild(typemods);

	try{
		while(1){
			pointermods->addChild(getPOINTER_MOD());
		}
	}
	catch (NoticeException ne){}
	result->addChild(pointermods);

	try{
		result->addChild(getCOMPOUND_NAME());
	}
	catch (NoticeException ne){
		
		
		
		if(typemods->getChildren().size() > 0){
			visitor.deleteTree(result);
			throw ParserException("Only TYPE_MODs, no TYPE name specified! " + currentToken.getPosition().toString());
		}

		if(pointermods->getChildren().size() > 0){
			visitor.deleteTree(result);
			throw ParserException("Only POINTER_MODs, no TYPE name specified! " + currentToken.getPosition().toString());
		}

		visitor.deleteTree(result);
		throw NoticeException("Not TYPE found!");
	}
	return result;
}

Node *getVARDECL_ELEM(){

	Node *result = nullptr;
	Node *name = nullptr;
	try {
		name = getCOMPOUND_NAME();
		result = new Node(Node::VARDECL_ELEM);
		result->addChild(name);
	}
	catch(NoticeException ne){
		throw NoticeException("No VARDECL_ELEM found!");
	}


	if(currentToken == Token(Token::OPERATOR, "=")){
		Node *op = new Node(Node::OPERATOR, "=");
		consume();
		op->addChild(new Node(*name));
		try {
			op->addChild(getASSIGNMENT());
		}
		catch (NoticeException ne){
			visitor.deleteTree(result);
			visitor.deleteTree(op);
			throw ParserException("Unfinished initialization expression at " + currentToken.getPosition().toString());
		}
		result->addChild(op);
	}

	return result;

	//	throw NoticeException("No VARDECL_ELEM found!");
}

Node *getVARDECL(){
	lock();
	Node *result = new Node(Node::VARDECL);
	//	Node *type = nullptr;

	try{
		result->addChild(getTYPE());
	}
	catch(NoticeException ne){
		visitor.deleteTree(result);
		recoil();
		throw NoticeException("No VARDECL!");
	}

	Node *tmp = nullptr;
	//	Node *op = nullptr;

	Node *list = nullptr;
	try{
		tmp = getVARDECL_ELEM();
		list = new Node(Node::VARDECL_LIST);
		list->addChild(tmp);
		try{
			while(currentToken == Token(Token::OPERATOR, ",")){
				consume();
				list->addChild(getVARDECL_ELEM());
			}
		}
		catch (NoticeException ne){
			recoil();
			throw ParserException ("Unfinished VARDECL_LIST at " + currentToken.getPosition().toString());
		}


	}
	catch(NoticeException ne){
		visitor.deleteTree(result);
		recoil();
		throw NoticeException("No VARDECL found!");
	}


	result->addChild(list);

	return result;
}



Node *getARG(){
	Node *result = new Node(Node::ARG);
	try{
		result->addChild(getTYPE());
	}
	catch(NoticeException ne){
		visitor.deleteTree(result);
		throw NoticeException("No TYPE for ARG found!");
	}

	//Node *var = nullptr;
	try{
		result->addChild(getVARDECL_ELEM());
	}
	catch(NoticeException ne){
	//	visitor.deleteTree(result);
	//	throw ParserException("No argument found ("+ ne.what()+") at " + currentToken.getPosition().toString());

	}

	return result;
}

Node *getFUNC_SIGN_ARGS(){
	if(!currentToken.typeEqualsTo(Token::BRACE_LEFT)){
		throw NoticeException("No FUNC_SING_ARGS found!");
	}

	consume();
	Node *result = new Node(Node::SIGN_ARGS);
	Node *list = nullptr;
	Node *typeList = nullptr;
	try{
		int counter = 0;
		list = new Node(Node::ARGS);
		typeList = new Node(Node::TYPE_LIST);
		list->addChild(getARG());
		typeList->addChild(visitor.clone(list->getChildren()[counter]->getChildren()[0]));
		++counter;
		try{
			while(currentToken == Token(Token::OPERATOR, ",")){
				consume();
				list->addChild(getARG());
				typeList->addChild(visitor.clone(list->getChildren()[counter]->getChildren()[0]));
				++counter;
			}
		}
		catch (NoticeException ne){
			visitor.deleteTree(list);
			visitor.deleteTree(typeList);
			visitor.deleteTree(result);
			throw ParserException ("Unfinished ARG at " + currentToken.getPosition().toString());
		}

	}
	catch(NoticeException ne){
		//empty for empty case;
	}

	result->addChild(typeList);
	result->addChild(list);

	if(!currentToken.typeEqualsTo(Token::BRACE_RIGHT)){
		visitor.deleteTree(result);
		throw ParserException("Missing BRACE_RIGHT in FUNC_SIGN_ARGS at " + currentToken.getPosition().toString());
	}

	consume();


	return result;

}

Node *getFUNC_SIGN(){
	if(!(currentToken == Token(Token::KEYWORD, "def"))){
		throw NoticeException ("No FUNC_SING found!");
	}

	consume();

	Node *result = new Node(Node::FUNC_SIGN);
	Node *type = nullptr;
	Node *name = nullptr;
	Node *args = nullptr;
	try{
		type = getTYPE();
		name = getCOMPOUND_NAME();
		args = getFUNC_SIGN_ARGS();
	}
	catch(NoticeException ne) {
		visitor.deleteTree(result);
		visitor.deleteTree(name);
		visitor.deleteTree(type);
		visitor.deleteTree(args);
		throw ParserException("Corrupted function signature (" + ne.what() + ")at " + currentToken.getPosition().toString());
	}

	result->addChild(name);
	result->addChild(type);
	result->addChild(args);


	
		
	return result;

}

Node *getOPERATOR(){

	try {
		return getVARDECL();
	}
	catch (NoticeException ne){}

	try {
		return getFUNC_DEF();
	}
	catch (NoticeException ne){}

	try {
		return getNON_EMPTY_EXPRESSION();
	}
	catch (NoticeException ne){}

	try {
		return getBLOCK();
	}
	catch (NoticeException ne){}

	try {
		return getSPECIAL();
	}
	catch (NoticeException ne){}	

	
	if(currentToken.typeEqualsTo(Token::SEMICOLON)){
		consume();
		return new Node(Node::OPERATOR);
	}
	

	throw NoticeException("No OPERATOR found!");
}

Node *getOPERATORS(){
	Node *result = new Node(Node::OPERATORS);

	try{

		while(true){
			while(currentToken.typeEqualsTo(Token::SEMICOLON)) {
				consume();
			}
			result->addChild(getOPERATOR());
		}		
			

	}
	catch(NoticeException ne){}

	if(result->getChildren().size() == 0){
		visitor.deleteTree(result);
		throw NoticeException ("No OPERATORS found!");
	}

	return result;
}

Node *getBLOCK(){
	if(!currentToken.typeEqualsTo(Token::CURL_LEFT)){
		throw NoticeException("No block found!");
	}
	consume();
	Node *result = new Node(Node::BLOCK);

	try {
		result->addChild(getOPERATORS());
	}
	catch (NoticeException &ne) {}

	if(!currentToken.typeEqualsTo(Token::CURL_RIGHT)){
		//cout << currentToken;
		throw ParserException("Unfinished block of code at " + currentToken.getPosition().toString());
	}
	consume();

	return result;
}

Node *getFUNC_DEF(){

	Node *result = nullptr;
	try{
		result = new Node(Node::FUNC_DEF);
		result->addChild(getFUNC_SIGN());
		result->addChild(getBLOCK());
		return result;
	}
	catch (NoticeException &ne){
		visitor.deleteTree(result);
		throw NoticeException ("No function found!");

	}
}

Node *getIF(){
	if(!(currentToken == Token(Token::KEYWORD, "if"))){
		throw ParserException("Trying to get IF statemetn without IF! Check functio getSPECIAL!!!");
	}

	consume();

	if(!currentToken.typeEqualsTo(Token::BRACE_LEFT)){
		throw NoticeException("Missing '(' at" + currentToken.getPosition().toString());
	}

	consume();

	Node *result = new Node(Node::IF);

	try {
		result->addChild(getEXPRESSION());
	}
	catch (NoticeException &ne){
		visitor.deleteTree(result);
		throw NoticeException("No EXPRESSION in IF!");
	}

	if(!currentToken.typeEqualsTo(Token::BRACE_RIGHT)){
		throw NoticeException("Missing ')' at " +  currentToken.getPosition().toString());
	}

	consume();

	try {
		result->addChild(getOPERATOR());
	}
	catch (NoticeException &ne){
		visitor.deleteTree(result);
		throw NoticeException("No OPERATOR in IF!");
	}

	if(currentToken == Token(Token::KEYWORD, "else")){
		consume();
		try {
		result->addChild(getOPERATOR());
		}
		catch (NoticeException &ne){
			visitor.deleteTree(result);
			throw NoticeException("Empty ELSE!");
		}
	}

	return result;
}

Node *getWHILE(){
	if(!(currentToken == Token(Token::KEYWORD, "while"))){
		throw ParserException("Trying to get WHILE statemetn without WHILE! Check functio getSPECIAL!!!");
	}

	consume();

	if(!currentToken.typeEqualsTo(Token::BRACE_LEFT)){
		throw NoticeException("Missing '(' at" + currentToken.getPosition().toString());
	}

	consume();

	Node *result = new Node(Node::WHILE);

	try {
		result->addChild(getEXPRESSION());
	}
	catch (NoticeException &ne){
		visitor.deleteTree(result);
		throw NoticeException("No EXPRESSION in WHILE!");
	}

	if(!currentToken.typeEqualsTo(Token::BRACE_RIGHT)){
		throw NoticeException("Missing ')' at " +  currentToken.getPosition().toString());
	}

	consume();

	try {
		result->addChild(getOPERATOR());
	}
	catch (NoticeException &ne){
		visitor.deleteTree(result);
		throw NoticeException("No OPERATOR in WHILE!");
	}

	if(currentToken == Token(Token::KEYWORD, "else")){
		consume();
		try {
		result->addChild(getOPERATOR());
		}
		catch (NoticeException &ne){
			visitor.deleteTree(result);
			throw NoticeException("Empty ELSE!");
		}
	}

	return result;
}

Node *getDOWHILE(){
	if(!(currentToken == Token(Token::KEYWORD, "do"))){
		throw ParserException("Trying to get DOWHILE statemetn without 'do'! Check functio getSPECIAL!!!");
	}

	consume();

	Node *result = new Node(Node::WHILE);

	try {
		result->addChild(getOPERATOR());
	}
	catch (NoticeException &ne){
		visitor.deleteTree(result);
		throw NoticeException("No OPERATOR in WHILE!");
	}

	if(!(currentToken == Token(Token::KEYWORD, "while"))){
		throw ParserException("Trying to get DOWHILE statemetn without 'while'! Check functio getSPECIAL!!!");
	}

	consume();

	if(!currentToken.typeEqualsTo(Token::BRACE_LEFT)){
		throw NoticeException("Missing '(' at" + currentToken.getPosition().toString());
	}

	consume();


	try {
		result->addChild(getEXPRESSION());
	}
	catch (NoticeException &ne){
		visitor.deleteTree(result);
		throw NoticeException("No EXPRESSION in IF!");
	}

	if(!currentToken.typeEqualsTo(Token::BRACE_RIGHT)){
		throw NoticeException("Missing ')' at " +  currentToken.getPosition().toString());
	}

	consume();



	return result;
}

Node *getFOR(){

	Node *result = nullptr;

	try{
		get(Token(Token::KEYWORD, "for"));
		result = new Node(Node::FOR);
		get(Token::BRACE_LEFT);
		result->addChild(getEXPRESSION());
		get(Token::SEMICOLON);
		result->addChild(getEXPRESSION());
		get(Token::SEMICOLON);
		result->addChild(getEXPRESSION());
		get(Token::BRACE_RIGHT);

		result->addChild(getOPERATOR());

	}	
	catch (NoticeException &ne){
		visitor.deleteTree(result);
		throw ParserException("Corrupted FOR (" + ne.what() + ") " + currentToken.getPosition().toString());
	}

	return result;

}

Node *getSPECIAL(){

	if(currentToken == Token(Token::KEYWORD, "if")){
		try{
			return getIF();
		}
		catch (NoticeException &ne){
			throw ParserException("Corrupted IF statemetn (" + ne.what() +") at " + currentToken.getPosition().toString());
		}
	}

	if(currentToken == Token(Token::KEYWORD, "while")){
		try{
			return getWHILE();
		}
		catch (NoticeException &ne){
			throw ParserException("Corrupted WHILE statemetn (" + ne.what() +") at " + currentToken.getPosition().toString());
		}
	}

	if(currentToken == Token(Token::KEYWORD, "do")){
		try{
			return getDOWHILE();
		}
		catch (NoticeException &ne){
			throw ParserException("Corrupted DOWHILE statemetn (" + ne.what() +") at " + currentToken.getPosition().toString());
		}
	}

	if(currentToken == Token(Token::KEYWORD, "for")){
		try{
			return getFOR();
		}
		catch (NoticeException &ne){
			throw ParserException("Corrupted FOR statemetn (" + ne.what() +") at " + currentToken.getPosition().toString());
		}
	}

	if(currentToken == Token(Token::KEYWORD, "else")){
		throw ParserException("ifless else at " + currentToken.getPosition().toString());
	}
	throw NoticeException("No special statemetn found!");
}


	
Node *getTree(){
	return this->tree;
}

void pushTree(){
	this->visitor.setTree(this->tree);
}

void buildTree(){
	this->tree = new Node(Node::PROGRAM);
	this->tree->addChild(getBEGIN());
	try {
		Node *tmp = getOPERATORS();
		this->tree->addChild(tmp);
	}
	catch(NoticeException ne){

	}
	this->tree->addChild(getEND());
}

};


#endif