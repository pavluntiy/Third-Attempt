
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


BasicNode* Parser::getCommaExpression(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getAssignment();
			}

			if(currentToken == Token(Token::OPERATOR, ",")){
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArg(left);
				op->addArg(getAssignment());
				return getCommaExpression(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException &ne){
			throw NoticeException("corrupted Comma expression (" + ne.what() + ") at" + currentToken.getPosition().toString());
		}
}


bool Parser::isAssignmentOp(){
	return 
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
	;
}

BasicNode* Parser::getAssignment(){
	BasicNode *left = nullptr;
	FunctionCallNode *op = nullptr;
	try{
		left = getTernary();
		if(isAssignmentOp()){
			op = new FunctionCallNode(currentToken);
			get(Token::OPERATOR);
			op->addArg(left);
			op->addArg(getTernary());
			return op;
		}
		else{
			return left;
		}
	}
	catch(NoticeException ne){
		throw NoticeException("Corrupted assignment operator  (" + ne.what() + ") at " + currentToken.getPosition().toString());
	}
}




BasicNode* Parser::getTernary(){
	FunctionCallNode *op = nullptr;
	BasicNode *cond = nullptr;
	try {
		cond = getInIsExpression();
		if(currentToken == Token(Token::OPERATOR, "?")){
			op = new FunctionCallNode(currentToken);
			get(Token::OPERATOR);
			op->addArg(cond);
			op->addArg(getTernary());
			if(currentToken != Token(Token::OPERATOR, ":")){
				throw ParserException("Corrupted ternary operator at " + currentToken.getPosition().toString());
			}
			get(Token::OPERATOR);
			op->addArg(getTernary());
			return op;
		}
		else {
			return cond;
		}
	}
	catch (NoticeException &ne){
		throw NoticeException("corrupted  ternary operator  (" + ne.what() + ") at " + currentToken.getPosition().toString());
	}
}

bool Parser::isInIsExpressionOp(){
	return 
			currentToken.getText() == "is"
			||
			currentToken.getText() == "in"
	;
}

BasicNode* Parser::getInIsExpression(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getLogicalOr();
			}

			if(isInIsExpressionOp()){
				op = new FunctionCallNode(currentToken);
				get(Token::KEYWORD);
				op->addArg(left);
				op->addArg(getLogicalOr());
				//return getInIsExpression(op);
				return op;
			}
			else{
				return left;
			}
		}
		catch (NoticeException ne){
			throw NoticeException("corrupted  Is/In expression  (" + ne.what() + ") at " + currentToken.getPosition().toString());
		}
}

BasicNode* Parser::getLogicalOr(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getLogicalAnd();
			}

			if(currentToken == Token(Token::OPERATOR, "||")){
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArg(left);
				op->addArg(getLogicalAnd());
				return getLogicalOr(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException &ne){
			throw NoticeException("corrupted '||' (" + ne.what() + ") at" + currentToken.getPosition().toString());
		}
}


BasicNode* Parser::getLogicalAnd(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getBitwiseOr();
			}

			if(currentToken == Token(Token::OPERATOR, "&&")){
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArg(left);
				op->addArg(getBitwiseOr());
				return getLogicalAnd(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException &ne){
			throw NoticeException("corrupted '&&' (" + ne.what() + ") at" + currentToken.getPosition().toString());
		}
}


BasicNode* Parser::getBitwiseOr(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getBitwiseXor();
			}

			if(currentToken == Token(Token::OPERATOR, "|")){
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArg(left);
				op->addArg(getBitwiseXor());
				return getBitwiseOr(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException &ne){
			throw NoticeException("corrupted '|' (" + ne.what() + ") at" + currentToken.getPosition().toString());
		}
}

BasicNode* Parser::getBitwiseXor(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getBitwiseAnd();
			}

			if(currentToken == Token(Token::OPERATOR, "^")){
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArg(left);
				op->addArg(getBitwiseAnd());
				return getBitwiseXor(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException &ne){
			throw NoticeException("corrupted '^' (" + ne.what() + ") at " + currentToken.getPosition().toString());
		}
}


BasicNode* Parser::getBitwiseAnd(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getComparison();
			}

			if(currentToken == Token(Token::OPERATOR, "&")){
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArg(left);
				op->addArg(getComparison());
				return getBitwiseAnd(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException &ne){
			throw NoticeException("corrupted '&' (" + ne.what() + ") at " + currentToken.getPosition().toString());
		}
}

bool Parser::isComparisonOp(){
	return 
			currentToken.getText() == "<="
			||
			currentToken.getText() == "<"
			||
			currentToken.getText() == ">="
			||
			currentToken.getText() == ">"
	;
}

BasicNode* Parser::getComparison(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getStrictComparison();
			}

			if(isComparisonOp()){
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArg(left);
				op->addArg(getStrictComparison());
				return getComparison(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException ne){
			throw NoticeException("corrupted  Comparison  (" + ne.what() + ") at " + currentToken.getPosition().toString());
		}
}



bool Parser::isStrictComparisonOp(){
	return 
			currentToken.getText() == "=="
			||
			currentToken.getText() == "="
	;
}

BasicNode* Parser::getStrictComparison(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getShift();
			}

			if(isStrictComparisonOp()){
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArg(left);
				op->addArg(getShift());
				return getStrictComparison(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException ne){
			throw NoticeException("corrupted Strict Comparison (" + ne.what() + ") at " + currentToken.getPosition().toString());
		}
}



bool Parser::isShiftOp(){
	return 
			currentToken.getText() == "<<"
			||
			currentToken.getText() == ">>"
			||
			currentToken.getText() == ">>>"
	;
}

BasicNode* Parser::getShift(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getExpr6();
			}

			if(isShiftOp()){
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArg(left);
				op->addArg(getExpr6());
				return getShift(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException ne){
			throw NoticeException("corrupted SHIFT (" + ne.what() + ") at " + currentToken.getPosition().toString());
		}
}


bool Parser::isExpr6Op(){
	return 
			currentToken.getText() == "+"
			||
			currentToken.getText() == "-"
	;
}

BasicNode* Parser::getExpr6(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getExpr7();
			}

			if(isExpr6Op()){
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArg(left);
				op->addArg(getExpr7());
				return getExpr6(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException ne){
			throw NoticeException("corrupted EXPR6 (" + ne.what() + ") at " + currentToken.getPosition().toString());
		}
}


bool Parser::isExpr7Op(){
	return 
			currentToken.getText() == "/"
			||
			currentToken.getText() == "%"
			||
			currentToken.getText() == "*"
	;
}

BasicNode* Parser::getExpr7(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getExpr8();
			}

			if(isExpr7Op()){
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArg(left);
				op->addArg(getExpr8());
				return getExpr7(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException ne){
			throw NoticeException("corrupted EXPR7 (" + ne.what() + ") at " + currentToken.getPosition().toString());
		}
}


bool Parser::isExpr8Op(){
	return currentToken.getText() == "**";
}

BasicNode* Parser::getExpr8(){
	BasicNode *left = nullptr;
	FunctionCallNode *op = nullptr;
	try{
		left = getExpr9();
		if(isExpr8Op()){
			op = new FunctionCallNode(currentToken);
			get(Token::OPERATOR);
			op->addArg(left);
			op->addArg(getExpr8());
			return op;
		}
		else{
			return left;
		}
	}
	catch(NoticeException ne){
		throw NoticeException("No EXPR8 found!");
	}
}

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
				throw ParserException("corrupted Access at " + currentToken.getPosition().toString());
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

bool Parser::isExpr9SuffixOp(){
	return
		currentToken.getText() == "--"
		||
		currentToken.getText() == "++"
	;
}

BasicNode* Parser::getExpr9Suffix(BasicNode *left){
	FunctionCallNode *op = nullptr;

	if(left == nullptr){
		left = getExpr10();
	}

		if(isExpr9SuffixOp()){
			auto tmpToken = Token(Token::OPERATOR, "_" +currentToken.getText());
			op = new FunctionCallNode(tmpToken);
			get(Token::OPERATOR);	
			op->addArg(getExpr9Suffix(left));
			return op;
		}

	return left;
}

bool Parser::isExpr9Op(){
	return 
		currentToken.getText() == "--"
		||
		currentToken.getText() == "++"
		||
		currentToken.getText() == "$"
		||
		currentToken.getText() == "@"
		||
		currentToken.getText() == "+"
		||
		currentToken.getText() == "-"
		||
		currentToken.getText() == "~"
		||
		currentToken.getText() == "!"
	;
}

BasicNode* Parser::getExpr9(){
	FunctionCallNode *op = nullptr;

	if(isExpr9Op()){
		op = new FunctionCallNode(currentToken);
		get(Token::OPERATOR);
		op->addArg(getExpr9());
		return op;
	}

	return getExpr9Suffix();
	// }
	// catch (NoticeException ne){
	// 	//cout << "OH YEAH!\n\n";
	// }

	// if(op == nullptr){
	// 	throw NoticeException("No EXPR9 found!");
	// }

	// return op;
}

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

	return getCommaExpression();
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
	try{
		get(Token::BEGIN);
		try {
			this->tree->addChild(getExpression());
		}
		catch(NoticeException &ne){
			cout << ne.what();
		}

		get(Token::END);
	}
	catch(NoticeException &ne){
		throw ParserException("Program structure corrupted: " + ne.what());
	}
}
