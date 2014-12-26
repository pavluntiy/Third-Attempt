
#include "parser.hpp"


Parser::Parser (Lexer &lexer):
		lexer(lexer)
	{
		this->currentPosition = 0;
		this->currentToken = this->lexer[0];
		this->history.push(currentPosition);
	}

void Parser::lock(){
	this->history.push(currentPosition);
}
void Parser::recoil(){

//	cout << "recoiling!!!!!!!!!!!!!!!!!!!!!\n";
	if(this->history.empty()){
		throw ParserException("Incorrect recoiling!");
	}

	this->currentPosition = this->history.top();
	this->history.pop();
	this->currentToken = this->lexer[this->currentPosition];
//	cout << currentToken << '\n';
}

void Parser::memoize(Node::Type type, int position, bool success, int jump){
	this->memo[make_pair(type, position)] = make_pair(success, jump);
}


void Parser::consume(){
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

void Parser::get(Token token){
	if(! (currentToken == token) ){
		throw NoticeException ("No " + token.toString() + " found, got " + currentToken.toString() + 
			"instead at " + currentToken.getPosition().toString());
	}
	consume();
}

void Parser::get(Token::Type type){
	if(! currentToken.typeEqualsTo(type) ){
		throw NoticeException ("No " + Token(type).toString() + " found, got " + currentToken.toString() + 
			"instead at " + currentToken.getPosition().toString());
	}
	consume();
}


// ExpressionNode* Parser::getCOMMA_EXPRESSION(FunctionCallNode *left){

// 		FunctionCallNode *op = nullptr, *right = nullptr;
// 		if(left == nullptr){
// 			try{
// 				left = getASSIGNMENT();
// 			}
// 			catch (NoticeException ne){
// 				throw NoticeException("No COMMA_EXPRESSION found!");
// 			}
// 		}

// 		if(currentToken != Token(Token::OPERATOR, ",")){
// 			return left;
// 		}
// 		get(Token::OPERATOR);

// 		try {
// 			right = getASSIGNMENT();
// 			op->addArg(left);
// 			op->addArg(right);
// 			return getCOMMA_EXPRESSION(op);
// 		}
// 		catch (NoticeException ne){
// 			throw ParserException("Operator for COMMA_EXPRESSION without operands at " + currentToken.getPosition().toString());
// 		}

// 		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

// }

// Node* Parser::getASSIGNMENT_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(
// 			currentToken.getText() == "="
// 			||
// 			currentToken.getText() == "*="
// 			||
// 			currentToken.getText() == "/="
// 			||
// 			currentToken.getText() == "%="
// 			||
// 			currentToken.getText() == "+="
// 			||
// 			currentToken.getText() == "-="
// 			||
// 			currentToken.getText() == ">>="
// 			||
// 			currentToken.getText() == ">>>="
// 			||
// 			currentToken.getText() == "<<="
// 			||
// 			currentToken.getText() == "&="
// 			||
// 			currentToken.getText() == "^="
// 			||
// 			currentToken.getText() == "|="
// 			||
// 			currentToken.getText() == "&&="
// 			||
// 			currentToken.getText() == "||="
// 			||
// 			currentToken.getText() == ":="
// 		){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 	}

// 	throw NoticeException("No EXPR8_OP found!");
// }

// Node* Parser::getASSIGNMENT(){
// 	Node *left = nullptr;
// 	Node *op = nullptr;
// 	Node *tmp = nullptr;

// 	try{
// 		left = getTERNARY();
// 	}
// 	catch (NoticeException ne){
// 	}

// 	try{
// 		op = getASSIGNMENT_OP();
// 		op->addChild(left);
// 		tmp = getASSIGNMENT();
// 		op->addChild(tmp);
// 		return op;
// 	}
// 	catch(NoticeException ne){
// 	}

// 	if(left == nullptr){
// 		throw NoticeException("No ASSIGNMENT found!");
// 	}
// 	return left;
// }

// Node *Parser::getTERNARY(){
// 	Node *op = nullptr;
// 	Node *cond = nullptr, *first = nullptr, *second = nullptr;

// 	try {
// 		cond = getL_AND();
// 	}
// 	catch (NoticeException ne){}

// 	if(cond == nullptr){
// 		throw NoticeException("No TERNARY found!");
// 	}


// 	if(currentToken.typeEqualsTo(Token::OPERATOR) && currentToken.getText() == "?"){
// 		consume();
// 		op = new Node(Node::OPERATOR, "?");
// 	}
// 	else {
// 		return cond;
// 	}

// 	first = getTERNARY();

// 	if(currentToken.typeEqualsTo(Token::OPERATOR) && currentToken.getText() == ":"){
// 		consume();
// 	}
// 	else{
// 		throw ParserException("Unfinished TERNARY operator at " + currentToken.getPosition().toString());
// 	}

// 	second = getTERNARY();

// 	op->addChild(cond);
// 	op->addChild(first);
// 	op->addChild(second);
// 	return op;
// }

// Node* Parser::getIS_IN_EXPRESSION_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::KEYWORD)){
// 		if(
// 			currentToken.getText() == "in"
// 			||
// 			currentToken.getText() == "is"
// 		){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 	}

// 	throw NoticeException("No IS_IN_EXPRESSION_OP found!");
// }

// Node* Parser::getIS_IN_EXPRESSION(Node *left){

// 		Node *op = nullptr, *right = nullptr;
// 		if(left == nullptr){
// 			try{
// 				left = getSHIFT();
// 			}
// 			catch (NoticeException ne){
// 				throw NoticeException("No IS_IN_EXPRESSION found!");
// 			}
// 		}

// 		try {
// 			op = getIS_IN_EXPRESSION_OP();
// 		}
// 		catch (NoticeException ne){
// 			return left;
// 		}

// 		try {
// 			right = getSHIFT();
// 			op->addChild(left);
// 			op->addChild(right);
// 			return getIS_IN_EXPRESSION(op);
// 		}
// 		catch (NoticeException ne){
// 			throw ParserException("Operator for IS_IN_EXPRESSION without operands at " + currentToken.getPosition().toString());
// 		}

// 		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

// }


// Node* Parser::getL_OR_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(
// 			currentToken.getText() == "||"
// 		){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 	}

// 	throw NoticeException("No L_OR_OP found!");
// }

// Node* Parser::getL_OR(Node *left){

// 		Node *op = nullptr, *right = nullptr;
// 		if(left == nullptr){
// 			try{
// 				left = getL_AND();
// 			}
// 			catch (NoticeException ne){
// 				throw NoticeException("No L_OR found!");
// 			}
// 		}

// 		try {
// 			op = getL_OR_OP();
// 		}
// 		catch (NoticeException ne){
// 			return left;
// 		}

// 		try {
// 			right = getL_AND();
// 			op->addChild(left);
// 			op->addChild(right);
// 			return getL_OR(op);
// 		}
// 		catch (NoticeException ne){
// 			throw ParserException("Operator for L_AND without operands at " + currentToken.getPosition().toString());
// 		}

// 		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

// }


// Node* Parser::getL_AND_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(
// 			currentToken.getText() == "&&"
// 		){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 	}

// 	throw NoticeException("No L_AND_OP found!");
// }

// Node* Parser::getL_AND(Node *left){

// 		Node *op = nullptr, *right = nullptr;
// 		if(left == nullptr){
// 			try{
// 				left = getB_OR();
// 			}
// 			catch (NoticeException ne){
// 				throw NoticeException("No L_AND found!");
// 			}
// 		}

// 		try {
// 			op = getL_AND_OP();
// 		}
// 		catch (NoticeException ne){
// 			return left;
// 		}

// 		try {
// 			right = getB_OR();
// 			op->addChild(left);
// 			op->addChild(right);
// 			return getL_AND(op);
// 		}
// 		catch (NoticeException ne){
// 			throw ParserException("Operator for L_AND without operands at " + currentToken.getPosition().toString());
// 		}

// 		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

// }

// Node* Parser::getCOMPARISION_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(
// 			currentToken.getText() == "=="
// 			||
// 			currentToken.getText() == "!="
// 		){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 	}

// 	throw NoticeException("No COMPARISION_OP found!");
// }
// Node* Parser::getCOMPARISION(Node *left){

// 		Node *op = nullptr, *right = nullptr;
// 		if(left == nullptr){
// 			try{
// 				left = getDIFF_COMPARISION();
// 			}
// 			catch (NoticeException ne){
// 				throw NoticeException("No COMPARISION found!");
// 			}
// 		}

// 		try {
// 			op = getCOMPARISION_OP();
// 		}
// 		catch (NoticeException ne){
// 			return left;
// 		}

// 		try {
// 			right = getDIFF_COMPARISION();
// 			op->addChild(left);
// 			op->addChild(right);
// 			return getCOMPARISION(op);
// 		}
// 		catch (NoticeException ne){
// 			throw ParserException("Operator for COMPARISION without operands at " + currentToken.getPosition().toString());
// 		}

// 		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

// }

// Node* Parser::getDIFF_COMPARISION_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(
// 			currentToken.getText() == "<="
// 			||
// 			currentToken.getText() == ">="
// 			||
// 			currentToken.getText() == "<"
// 			||
// 			currentToken.getText() == ">"
// 		){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 	}

// 	throw NoticeException("No DIFF_COMPARISION_OP found!");
// }
// Node* Parser::getDIFF_COMPARISION(Node *left){

// 		Node *op = nullptr, *right = nullptr;
// 		if(left == nullptr){
// 			try{
// 				left = getIS_IN_EXPRESSION();
// 			}
// 			catch (NoticeException ne){
// 				throw NoticeException("No DIFF_COMPARISION found!");
// 			}
// 		}

// 		try {
// 			op = getDIFF_COMPARISION_OP();
// 		}
// 		catch (NoticeException ne){
// 			return left;
// 		}

// 		try {
// 			right = getIS_IN_EXPRESSION();
// 			op->addChild(left);
// 			op->addChild(right);
// 			return getDIFF_COMPARISION(op);
// 		}
// 		catch (NoticeException ne){
// 			throw ParserException("Operator for DIFF_COMPARISION without operands at " + currentToken.getPosition().toString());
// 		}

// 		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

// }

// Node* Parser::getB_OR_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(
// 			currentToken.getText() == "|"
// 		){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 	}

// 	throw NoticeException("No B_OR_OP found!");
// }
// Node* Parser::getB_OR(Node *left){

// 		Node *op = nullptr, *right = nullptr;
// 		if(left == nullptr){
// 			try{
// 				left = getB_XOR();
// 			}
// 			catch (NoticeException ne){
// 				throw NoticeException("No B_XOR found!");
// 			}
// 		}

// 		try {
// 			op = getB_OR_OP();
// 		}
// 		catch (NoticeException ne){
// 			return left;
// 		}

// 		try {
// 			right = getB_XOR();
// 			op->addChild(left);
// 			op->addChild(right);
// 			return getB_OR(op);
// 		}
// 		catch (NoticeException ne){
// 			throw ParserException("Operator for B_OR without operands at " + currentToken.getPosition().toString());
// 		}

// 		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

// }

// Node* Parser::getB_XOR_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(
// 			currentToken.getText() == "^"
// 		){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 	}

// 	throw NoticeException("No B_XOR_OP found!");
// }
// Node* Parser::getB_XOR(Node *left){

// 		Node *op = nullptr, *right = nullptr;
// 		if(left == nullptr){
// 			try{
// 				left = getB_AND();
// 			}
// 			catch (NoticeException ne){
// 				throw NoticeException("No B_XOR found!");
// 			}
// 		}

// 		try {
// 			op = getB_XOR_OP();
// 		}
// 		catch (NoticeException ne){
// 			return left;
// 		}

// 		try {
// 			right = getB_AND();
// 			op->addChild(left);
// 			op->addChild(right);
// 			return getB_XOR(op);
// 		}
// 		catch (NoticeException ne){
// 			throw ParserException("Operator for B_XOR without operands at " + currentToken.getPosition().toString());
// 		}

// 		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

// }

// Node* Parser::getB_AND_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(
// 			currentToken.getText() == "&"
// 		){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 	}

// 	throw NoticeException("No B_AND_OP found!");
// }
// Node* Parser::getB_AND(Node *left){

// 		Node *op = nullptr, *right = nullptr;
// 		if(left == nullptr){
// 			try{
// 				left = getCOMPARISION();
// 			}
// 			catch (NoticeException ne){
// 				throw NoticeException("No B_AND found!");
// 			}
// 		}

// 		try {
// 			op = getB_AND_OP();
// 		}
// 		catch (NoticeException ne){
// 			return left;
// 		}

// 		try {
// 			right = getCOMPARISION();
// 			op->addChild(left);
// 			op->addChild(right);
// 			return getB_AND(op);
// 		}
// 		catch (NoticeException ne){
// 			throw ParserException("Operator for B_AND without operands at " + currentToken.getPosition().toString());
// 		}

// 		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

// }

// Node* Parser::getSHIFT_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(
// 			currentToken.getText() == "<<"
// 			||
// 			currentToken.getText() == ">>"
// 			||
// 			currentToken.getText() == ">>>"
// 		){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 	}

// 	throw NoticeException("No SHIFT_OP found!");
// }
// Node* Parser::getSHIFT(Node *left){

// 		Node *op = nullptr, *right = nullptr;
// 		if(left == nullptr){
// 			try{
// 				left = getEXPR6();
// 			}
// 			catch (NoticeException ne){
// 				throw NoticeException("No EXPR6 found!");
// 			}
// 		}

// 		try {
// 			op = getSHIFT_OP();
// 		}
// 		catch (NoticeException ne){
// 			return left;
// 		}

// 		try {
// 			right = getEXPR6();
// 			op->addChild(left);
// 			op->addChild(right);
// 			return getSHIFT(op);
// 		}
// 		catch (NoticeException ne){
// 			throw ParserException("Operator for SHIFT without operands at " + currentToken.getPosition().toString());
// 		}

// 		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

// }

// Node* Parser::getEXPR6_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(
// 			currentToken.getText() == "+"
// 			||
// 			currentToken.getText() == "-"
// 		){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 	}

// 	throw NoticeException("No EXPR6_OP found!");
// }
// Node* Parser::getEXPR6(Node *left){

// 		Node *op = nullptr, *right = nullptr;
// 		if(left == nullptr){
// 			try{
// 				left = getEXPR7();
// 			}
// 			catch (NoticeException ne){
// 				throw NoticeException("No EXPR6 found!");
// 			}
// 		}

// 		try {
// 			op = getEXPR6_OP();
// 		}
// 		catch (NoticeException ne){
// 			return left;
// 		}

// 		try {
// 			right = getEXPR7();
// 			op->addChild(left);
// 			op->addChild(right);
// 			return getEXPR6(op);
// 		}
// 		catch (NoticeException ne){
// 			throw ParserException("Operator for EXPR6 without operands at " + currentToken.getPosition().toString());
// 		}

// 		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

// }

// Node* Parser::getEXPR7_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(
// 			currentToken.getText() == "/"
// 			||
// 			currentToken.getText() == "%"
// 			||
// 			currentToken.getText() == "*"
// 		){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 	}

// 	throw NoticeException("No EXPR7_OP found!");
// }
// Node* Parser::getEXPR7(Node *left){

// 		Node *op = nullptr, *right = nullptr;
// 		if(left == nullptr){
// 			try{
// 				left = getEXPR8();
// 			}
// 			catch (NoticeException ne){
// 				throw NoticeException("No EXPR7 found!");
// 			}
// 		}

// 		try {
// 			op = getEXPR7_OP();
// 		}
// 		catch (NoticeException ne){
// 			return left;
// 		}

// 		try {
// 			right = getEXPR8();
// 			op->addChild(left);
// 			op->addChild(right);
// 			return getEXPR7(op);
// 		}
// 		catch (NoticeException ne){
// 			throw ParserException("Operator for EXPR7 without operands at " + currentToken.getPosition().toString());
// 		}

// 		throw ParserException("How did I get here!?!?" +  currentToken.getPosition().toString());

// }

// Node* Parser::getBEGIN(){
// 	if(currentToken.typeEqualsTo(Token::BEGIN)){
// 		consume();
// 		return new Node(Node::BEGIN);
// 	}
// 	else {
// 		throw ParserException("No BEGIN found!");
// 	}
// }

// Node* Parser::getEND(){
// 	if(currentToken.typeEqualsTo(Token::END)){
// 		consume();
// 		return new Node(Node::END);
// 	}
// 	else {
// 	//	cout << currentToken;
// 		throw ParserException("No END found, got " + currentToken.toString() + " instead");
// 	}
// }

// Node* Parser::getINT(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::INT)){
// 		result = new Node(Node::INT, currentToken.getText());
// 		consume();
// 		return result;
// 	}
// 	else {
// 		throw NoticeException("No INT found!");
// 	}
// }


// Node* Parser::getFLOAT(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::FLOAT)){
// 		result = new Node(Node::FLOAT, currentToken.getText());
// 		consume();
// 		return result;
// 	}
// 	else {
// 		throw NoticeException("No FLOAT found!");
// 	}
// }

// Node* Parser::getCHAR(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::CHAR)){
// 		result = new Node(Node::CHAR, currentToken.getText());
// 		consume();
// 		return result;
// 	}
// 	else {
// 		throw NoticeException("No CHAR found!");
// 	}
// }

// Node* Parser::getSTRING(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::STRING)){
// 		result = new Node(Node::STRING, currentToken.getText());
// 		consume();
// 		return result;
// 	}
// 	else {
// 		throw NoticeException("No CHAR found!");
// 	}
// }



// 	throw NoticeException("No EXPR10_OP found!");
// }

// Node* Parser::getEXPR8_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(currentToken.getText() == "**"){
// 			result = new Node(Node::OPERATOR, "**");
// 			consume();
// 			return result;
// 		}

// 	}

// 	throw NoticeException("No EXPR8_OP found!");
// }

// Node* Parser::getEXPR8(){
// 	Node *left = nullptr;
// 	Node *op = nullptr;
// 	Node *tmp = nullptr;

// 	try{
// 		left = getEXPR9();
// 	}
// 	catch (NoticeException ne){
// 	}

// 	try{
// 		op = getEXPR8_OP();
// 		op->addChild(left);
// 		tmp = getEXPR8();
// 		op->addChild(tmp);
// 		return op;
// 	}
// 	catch(NoticeException ne){
// 	}

// 	if(left == nullptr){
// 		throw NoticeException("No EXPR8 found!");
// 	}
// 	return left;
// }

BasicNode* Parser::getAtom(){
	if(currentToken.typeEqualsTo(Token::BRACE_LEFT)){
		get(Token::BRACE_LEFT);
		BasicNode *result = getExpression();
		if(!currentToken.typeEqualsTo(Token::BRACE_RIGHT)){
			throw ParserException("Missing BRACE_RIGHT at " + currentToken.getPosition().toString());
		}
		get(Token::BRACE_RIGHT);
		return result;
	}
	try{
		return getName();
	}
	catch(NoticeException &ne){

	}
	//	cout << "YEAH!\n";
	try{
		return getValue();
	}
	catch(NoticeException &ne){

	}

	throw NoticeException("No ATOM found at " + currentToken.getPosition().toString() + " got " + currentToken.toString());

}

BasicNode* Parser::getName(){
	CompoundNameNode *result = nullptr;
	if(currentToken.typeEqualsTo(Token::NAME)){
		result = new CompoundNameNode(currentToken.getText());
		consume();
		//	
		return result;
	}

	throw NoticeException("No NAME");
}

// Node* Parser::getFUNCARGS(){
// 	Node  *tmp = nullptr, *result = nullptr;

// 	result = new Node(Node::FUNCARGS);


// 	try{
// 		tmp = getASSIGNMENT();
// 	}
// 	catch(NoticeException ne){
// 		return result;
// 	};

// 	result->addChild(tmp);

// 	while(currentToken == Token(Token::OPERATOR, ",")){
// 		consume();
// 		tmp = getASSIGNMENT();
// 		result->addChild(tmp);
// 	}
	
// 	return result;
// }

BasicNode* Parser::getFunctionCalls(BasicNode *left){

		FunctionCallNode *result = nullptr;
		if(currentToken.typeEqualsTo(Token::BRACE_LEFT)){
					get(Token::BRACE_LEFT);
					try{	
							result = new FunctionCallNode(left);
							result->addArg(getExpression());
							cout << currentToken;
							while(currentToken == Token(Token::OPERATOR, ",")){
								//cout << "mimi!";
								get(Token::OPERATOR);
								result->addArg(getExpression());
								//SHould I check here for 'foo(arg1, )'
								//cout << "hshs!";
							}
					}
					catch (NoticeException  &ne){};
					if(!currentToken.typeEqualsTo(Token::BRACE_RIGHT)){
						throw ParserException("Missed BRACE_RIGHT at " + currentToken.getPosition().toString());
					}
					get(Token::BRACE_RIGHT);

					//try {
						return getFunctionCalls(result);
					// }
					// catch(NoticeException ne){

					// }
		}
	
		return left;
}

BasicNode* Parser::getAccessArgs(){
	try {
		return getExpression();
	}
	catch (NoticeException ne){

	}

	throw NoticeException("No FUNCARGS found!");
}

BasicNode* Parser::getAccesses(BasicNode *left){

		FunctionCallNode *result = nullptr;
		if(currentToken.typeEqualsTo(Token::BRACKET_LEFT)){
			try {
					get(Token::BRACKET_LEFT);
					result = new FunctionCallNode(Token(Token::OPERATOR, "[]"));
					result->addArg(left);
					result->addArg(getAccessArgs());
					if(!currentToken.typeEqualsTo(Token::BRACKET_RIGHT)){
						throw ParserException("Missed BRACKET_RIGHT at " + currentToken.getPosition().toString());
					}
					get(Token::BRACKET_RIGHT);

					return getAccesses(result);
			}
			catch(NoticeException ne){

			}
		}
	
		return left;
}

BasicNode* Parser::getBraced(BasicNode *left){
	if(left == nullptr){
		throw ParserException("Unexpected BRACE_LEFT or BRACKET_RIGHT at " + currentToken.getPosition().toString());
	}

	while(currentToken.typeEqualsTo(Token::BRACE_LEFT) || currentToken.typeEqualsTo(Token::BRACKET_LEFT)){
		if(currentToken.typeEqualsTo(Token::BRACE_LEFT)){
			left = getFunctionCalls(left);
		}
		else {
			left = getAccesses(left);
		}
	}

	return left;
}

// Node* Parser::getEXPR9_OP_SUFFIX(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(currentToken.getText() == "--"){
// 			result = new Node(Node::OPERATOR, "_--");
// 			consume();
// 		//	cout << currentToken.getText();
// 			return result;
// 		}

// 		if(currentToken.getText() == "++"){
// 			result = new Node(Node::OPERATOR, "_++");
// 			consume();
// 			return result;
// 		}
// 	}

// 	throw NoticeException("No EXPR9_OP_SUFFIX found , got + " + currentToken.toString());
// }

// Node* Parser::getEXPR9_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(currentToken.getText() == "--"){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 		//	cout << currentToken.getText();
// 			return result;
// 		}

// 		if(currentToken.getText() == "++"){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 		if(currentToken.getText() == "$"){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 		if(currentToken.getText() == "@"){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 		if(currentToken.getText() == "+"){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 		if(currentToken.getText() == "-"){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 		if(currentToken.getText() == "~"){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}

// 		if(currentToken.getText() == "!"){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}
// 	}

// 	throw NoticeException("No EXPR9_OP found, got + " + currentToken.toString());
// }

// Node* Parser::getEXPR9_SUFFIX(Node *left){
// 	Node *op = nullptr;
// 	Node *tmp = nullptr;
//   //	cout << "AT ENTER LEFT IS " << left << '\n'; 
// 	if(left == nullptr){
// 		try{
//   //			cout << "#######\n";
//   //			cout << currentToken;
// 			left = getEXPR10();
// 		}
// 		catch (NoticeException ne){
// 		//	throw ParserException("Strange stuff at " + currentToken.getPosition().toString() + ";\n got " + currentToken.toString());
// 		}
// 	}

//   //	cout << "ONGTWSEDFVGJS\n";
// 	try{
// 		op = getEXPR9_OP_SUFFIX();
// 		tmp = getEXPR9_SUFFIX(left);
// 		op->addChild(tmp);
// 		return op;
// 	}
// 	catch(NoticeException ne){
//  //		cout << ne.what();
// 	}
//  //	cout << "LEFT IS " << left << '\n';
// 	return left;
// }

// Node* Parser::getEXPR9(){
// 	//cout << "TEWEGSDF|\n";
// 	Node *son= nullptr;
// 	Node *op = nullptr;
// 	try {
// 		op = getEXPR9_OP();
// 		son = getEXPR9();
// 		op->addChild(son);
// 		return op;
// 	}
// 	catch (NoticeException ne){
// 		//cout << ne.what();
// 		//cout << "ppppp\n";
// 		//cout << currentToken << '\n';
// 	}

// 	try {
// 	//	cout << "eeeee\n";
// 	//	cout << currentToken;
// 		//cout << "currentPosition: " << currentPosition << '\n';
// 		return getEXPR9_SUFFIX();
// 	}
// 	catch (NoticeException ne){
// 		//cout << "OH YEAH!\n\n";
// 	}

// 	if(op == nullptr){
// 		throw NoticeException("No EXPR9 found!");
// 	}

// 	return op;
// }

bool Parser::isExpr10Op(){

	return 
		currentToken == Token(Token::OPERATOR, ".")
		||
		currentToken == Token(Token::OPERATOR, "->")
	;
}

BasicNode* Parser::getExpr10(BasicNode *left){

	BasicNode *right = nullptr;
	FunctionCallNode *op = nullptr;

	if(left == nullptr){
		try{
			left = getAtom();
			//
			left = getBraced(left);
			//cout << "Tyakulli!";
				if(isExpr10Op()){
					op = new FunctionCallNode(currentToken);
					get(Token::OPERATOR);
					right = getAtom();
					op->addArg(left);
					op->addArg(right);
					return getExpr10(op);
				}
				else {

					return left;
				}
			}
			catch(NoticeException &ne){
				//cout << ne.what() <<"OMg #1";
			}

	}
	else {				
			try{
				left = getBraced(left);

				if(isExpr10Op()){
					op = new FunctionCallNode(currentToken);
					get(Token::OPERATOR);
					right = getAtom();
					op->addArg(left);
					op->addArg(right);
					return getExpr10(op);
				}
				else {
					//cout << "OMg #2" << currentToken;
					return getBraced(left);
				}
			}
			catch(NoticeException &ne){
				//cout << ne.what() << "OMg #3";
				return getAtom();
			}
			return left;
	}

	throw NoticeException("No EXPR10 found!");
}

bool Parser::isValue(){
	return 
		currentToken.typeEqualsTo(Token::INT)
		||
		currentToken.typeEqualsTo(Token::FLOAT)
		||
		currentToken.typeEqualsTo(Token::CHAR)
		||
		currentToken.typeEqualsTo(Token::STRING)
	;
}

BasicNode* Parser::getValue(){

	if(isValue()){
		auto tmp = new ValueNode(currentToken.getType(), currentToken.getText());
		consume();
		return tmp;
	}

	throw NoticeException("No VALUE found!");
}

// Node* Parser::getNON_EMPTY_EXPRESSION(){

// 	lock();
// 	try {

// 		return getCOMMA_EXPRESSION();
// 	}
// 	catch (NoticeException ne){
// 		recoil();
// 	}

// 	throw NoticeException("No NON_EMPTY_EXPRESSION found!");
// }

BasicNode* Parser::getExpression(){

	return getExpr10();
}

// Node* Parser::getTYPE_MOD(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::KEYWORD)){
// 		if(
// 			currentToken.getText() == "const"
// 			||
// 			currentToken.getText() == "long"
// 			||
// 			currentToken.getText() == "short"
// 			||
// 			currentToken.getText() == "unsigned"
// 			||
// 			currentToken.getText() == "signed"
// 			||
// 			currentToken.getText() == "volatile"
// 			||
// 			currentToken.getText() == "extern"


// 		){
// 			result = new Node(Node::TYPE_MOD, currentToken.getText());
// 			consume();
// 			return result;
// 		}
// 	}

// 		throw NoticeException("No TYPE_MOD found!");

// }

// Node* Parser::getPOINTER_MOD(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(
// 			currentToken.getText() == "@"
// 		){
// 			result = new Node(Node::POINTER_MOD, currentToken.getText());
// 			consume();
// 			return result;
// 		}
// 	}

// 	if(currentToken.typeEqualsTo(Token::KEYWORD)){
// 		if(
// 			currentToken.getText() == "ref"
// 		){
// 			result = new Node(Node::POINTER_MOD, currentToken.getText());
// 			consume();
// 			return result;
// 		}
// 	}

// 		throw NoticeException("No POINTER_MOD found!");
// }

// Node* Parser::getTYPENAME_OP(){
// 	Node *result = nullptr;
// 	if(currentToken.typeEqualsTo(Token::OPERATOR)){
// 		if(currentToken.getText() == "."){
// 			result = new Node(Node::OPERATOR, currentToken.getText());
// 			consume();
// 			return result;
// 		}
// 	}

// 	throw NoticeException ("No '.' for COMPOUND_NAME found!");
// }

// CompounNameNode* Parser::getCOMPOUND_NAME(Node *left){
// 	Node *right = nullptr;
// 	Node *op = nullptr;
// 	if(left == nullptr){
// 		try{
// 			left = getNAME();
// 			try{
// 				op = getTYPENAME_OP();
// 				op->addChild(left);
// 				right = getNAME();
// 				op->addChild(right);
// 				return getCOMPOUND_NAME(op);
// 			}
// 			catch(NoticeException ne){
// 				if(op != nullptr && right == nullptr){
// 					//visitor.deleteTree (op);
// 					// visitor.deleteTree(left);
// 					// visitor.deleteTree(right);
// 					throw ParserException("Strange '.' in name at " + currentToken.getPosition().toString());
// 				}
// 				return left;
// 			}
// 		}
// 		catch(NoticeException ne){

// 		}
// 	}
// 	else {	
// 			try{
// 				op = getTYPENAME_OP();
// 				right = getNAME();
// 				op->addChild(left);
// 				op->addChild(right);
// 				return getCOMPOUND_NAME(op);
// 			}
// 			catch(NoticeException ne){
// 				//visitor.deleteTree (op);
// 				//visitor.deleteTree(right);
// 			}
// 			return left;
// 	}

// 	try {
// 		return getNAME();
// 	}
// 	catch(NoticeException ne){

// 	}

// 	throw NoticeException("No COMPOUND_NAME found!");
// }

// Node* Parser::getTYPE(){
// 	Node *result = new Node(Node::TYPE);
// 	Node *typemods = new Node(Node::TYPE_MOD);
// 	Node *pointermods = new Node(Node::POINTER_MOD);

// 	try{
// 		while(1){
// 			typemods->addChild(getTYPE_MOD());
// 		}
// 	}
// 	catch (NoticeException ne){}

// 	result->addChild(typemods);

// 	try{
// 		while(1){
// 			pointermods->addChild(getPOINTER_MOD());
// 		}
// 	}
// 	catch (NoticeException ne){}
// 	result->addChild(pointermods);

// 	try{
// 		result->addChild(getCOMPOUND_NAME());
// 	}
// 	catch (NoticeException ne){
		
		
		
// 		if(typemods->getChildren().size() > 0){
// 			//visitor.deleteTree(result);
// 			throw ParserException("Only TYPE_MODs, no TYPE name specified! " + currentToken.getPosition().toString());
// 		}

// 		if(pointermods->getChildren().size() > 0){
// 			//visitor.deleteTree(result);
// 			throw ParserException("Only POINTER_MODs, no TYPE name specified! " + currentToken.getPosition().toString());
// 		}

// 		//visitor.deleteTree(result);
// 		throw NoticeException("Not TYPE found!");
// 	}
// 	return result;
// }

// Node* Parser::getVARDECL_ELEM(){

// 	Node *result = nullptr;
// 	Node *name = nullptr;
// 	try {
// 		name = getCOMPOUND_NAME();
// 		result = new Node(Node::VARDECL_ELEM);
// 		result->addChild(name);
// 	}
// 	catch(NoticeException ne){
// 		throw NoticeException("No VARDECL_ELEM found!");
// 	}


// 	if(currentToken == Token(Token::OPERATOR, "=")){
// 		Node *op = new Node(Node::OPERATOR, "=");
// 		consume();
// 		op->addChild(new Node(*name));
// 		try {
// 			op->addChild(getASSIGNMENT());
// 		}
// 		catch (NoticeException ne){
// 			//visitor.deleteTree(result);
// 			//visitor.deleteTree(op);
// 			throw ParserException("Unfinished initialization expression at " + currentToken.getPosition().toString());
// 		}
// 		result->addChild(op);
// 	}

// 	return result;

// 	//	throw NoticeException("No VARDECL_ELEM found!");
// }

// Node* Parser::getVARDECL(){
// 	lock();
// 	Node *result = new Node(Node::VARDECL);
// 	//	Node *type = nullptr;

// 	try{
// 		result->addChild(getTYPE());
// 	}
// 	catch(NoticeException ne){
// 		//visitor.deleteTree(result);
// 		recoil();
// 		throw NoticeException("No VARDECL!");
// 	}

// 	Node *tmp = nullptr;
// 	//	Node *op = nullptr;

// 	Node *list = nullptr;
// 	try{
// 		tmp = getVARDECL_ELEM();
// 		list = new Node(Node::VARDECL_LIST);
// 		list->addChild(tmp);
// 		try{
// 			while(currentToken == Token(Token::OPERATOR, ",")){
// 				consume();
// 				list->addChild(getVARDECL_ELEM());
// 			}
// 		}
// 		catch (NoticeException ne){
// 			recoil();
// 			throw ParserException ("Unfinished VARDECL_LIST at " + currentToken.getPosition().toString());
// 		}


// 	}
// 	catch(NoticeException ne){
// 		//visitor.deleteTree(result);
// 		recoil();
// 		throw NoticeException("No VARDECL found!");
// 	}


// 	result->addChild(list);

// 	return result;
// }



// Node* Parser::getARG(){
// 	Node *result = new Node(Node::ARG);
// 	try{
// 		result->addChild(getTYPE());
// 	}
// 	catch(NoticeException ne){
// 		//visitor.deleteTree(result);
// 		throw NoticeException("No TYPE for ARG found!");
// 	}

// 	//Node *var = nullptr;
// 	try{
// 		result->addChild(getVARDECL_ELEM());
// 	}
// 	catch(NoticeException ne){
// 	//	visitor.deleteTree(result);
// 	//	throw ParserException("No argument found ("+ ne.what()+") at " + currentToken.getPosition().toString());

// 	}

// 	return result;
// }


BasicNode* Parser::getTree(){
	return this->tree;
}

void Parser::pushTree(){

}

void Parser::buildTree(){
	this->tree = new ProgramNode();
	get(Token::BEGIN);
	try {
		BasicNode *tmp = getExpression();
		this->tree->addChild(tmp);
		//cout << tmp;
		//	cout << reinterpret_cast<CompoundNameNode*>(tmp)->getLeft() << "Here I am!";
	}
	catch(NoticeException &ne){
		cout << ne.what();
	}

	get(Token::END);
}
