
#include "parser.hpp"


Parser::Parser (Lexer &lexer):
	lexer(lexer)
	{
		this->currentPosition = 0;
		this->currentToken = this->lexer[0];
		this->history.push(currentPosition);
		this->deleteVisitor = new DeleteVisitor;
	}
Parser::~Parser(){
	delete this->deleteVisitor;
}

void Parser::lock(){
	this->history.push(currentPosition);
}

void Parser::recoil(){
	if(this->history.empty()){
		throw ParserException("Incorrect recoiling!", currentToken.getPosition());
	}

	this->currentPosition = this->history.top();
	this->history.pop();
	this->currentToken = this->lexer[this->currentPosition];
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
			if(currentToken.typeEqualsTo(Token::ERROR)){
				throw ParserException("Problems with lexer:\n" + currentToken.getText(), currentToken.getPosition());
			}
			++this->currentPosition;
			this->currentToken = this->lexer[this->currentPosition];

		}	
	}
}

void Parser::tryAndGet(Token::Type type){
	if(currentToken.typeEqualsTo(type)){
		consume();
	}
}

void Parser::tryAndGet(Token token){
	if(currentToken == token){
		consume();
	}
}

void Parser::get(Token token){
	if(currentToken != token){
		throw NoticeException ("No " + token.toString() + " found, got " + currentToken.toString() + 
			"instead", currentToken.getPosition());
	}
	consume();
}

void Parser::get(Token::Type type){
	if(!currentToken.typeEqualsTo(type)){
		throw NoticeException ("No " + Token(type).toString() + " found, got " + currentToken.toString() + 
			"instead", currentToken.getPosition());
	}
	consume();
}


BasicNode* Parser::getCommaExpression(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		bool commaFound = false;
		try {
			if(left == nullptr){
				left = getAssignment();
			}

			if(currentToken == Token(Token::OPERATOR, ",")){
				commaFound = true;
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArgument(left);
				op->addArgument(getAssignment());
				return getCommaExpression(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException &ne){
			

			if(commaFound){
				if(op){
					op->accept(this->deleteVisitor);
				}
				throw ParserException("corrupted Comma expression", currentToken.getPosition());
			}
			throw NoticeException("Comma expression not found");
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
	bool opFound = false;
	string opName = "";
	try{
		left = getTernary();
		if(isAssignmentOp()){
			opFound = true;
			opName = currentToken.getText();
			op = new FunctionCallNode(currentToken);
			get(Token::OPERATOR);
			op->addArgument(left);
			op->addArgument(getTernary());
			return op;
		}
		else{
			return left;
		}
	}
	catch(NoticeException ne){
		if(opFound){
			if(op){
				op->accept(this->deleteVisitor);
			}
			throw ParserException("Found '" + opName + "', but missing an argument ", currentToken.getPosition());
		}
		throw NoticeException("No assignment found!");
	}
}




BasicNode* Parser::getTernary(){
	FunctionCallNode *op = nullptr;
	BasicNode *cond = nullptr;
	bool thenBranchFound = false, questionMarkFound = false;
	try {
		cond = getInIsExpression();
		if(currentToken == Token(Token::OPERATOR, "?")){
			questionMarkFound = true;
			op = new FunctionCallNode(currentToken);
			get(Token::OPERATOR);
			op->addArgument(cond);
			op->addArgument(getTernary());
			thenBranchFound = true;
			if(currentToken != Token(Token::OPERATOR, ":")){
				throw ParserException("No 'else' branch found for ternary ", currentToken.getPosition());
			}
			get(Token::OPERATOR);
			op->addArgument(getTernary());
			return op;
		}
		else {
			return cond;
		}
	}
	catch (NoticeException &ne){
		
		if(thenBranchFound){
			if(op){
				op->accept(this->deleteVisitor);
			}
			throw ParserException("No 'else' branch found for ternary ", currentToken.getPosition());
		}
		if(questionMarkFound){
			if(op){
				op->accept(this->deleteVisitor);
			}
			throw ParserException("No 'then' branch found for ternary ", currentToken.getPosition());
		}
		throw NoticeException("No ternary found");
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
		bool opFound = false;
		string opName = "";
		try {
			if(left == nullptr){
				left = getLogicalOr();
			}

			if(isInIsExpressionOp()){
				op = new FunctionCallNode(currentToken);
				opFound = true;
				opName = currentToken.getText();
				get(Token::KEYWORD);
				op->addArgument(left);
				op->addArgument(getLogicalOr());
				//return getInIsExpression(op);
				return op;
			}
			else{
				return left;
			}
		}
		catch (NoticeException ne){		
			if(opFound){
				if(op){
					op->accept(this->deleteVisitor);
				}
				throw ParserException("Found '" + opName + "', but missing an argument ", currentToken.getPosition());
			}
			throw NoticeException("no Is/In found");
		}
}

BasicNode* Parser::getLogicalOr(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		bool opFound = false;
		string opName = "";
		try {
			if(left == nullptr){
				left = getLogicalAnd();
			}

			if(currentToken == Token(Token::OPERATOR, "||")){
				opFound = true;
				opName = currentToken.getText();
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArgument(left);
				op->addArgument(getLogicalAnd());
				return getLogicalOr(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException &ne){
			if(opFound){
				if(op){
					op->accept(this->deleteVisitor);
				}
				throw ParserException("Found '" + opName + "', but missing an argument ", currentToken.getPosition());
			}
			throw NoticeException("No logical or found");
		}
}


BasicNode* Parser::getLogicalAnd(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		bool opFound = false;
		string opName = "";
		try {
			if(left == nullptr){
				left = getBitwiseOr();
			}

			if(currentToken == Token(Token::OPERATOR, "&&")){
				opFound = true;
				opName = currentToken.getText();
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArgument(left);
				op->addArgument(getBitwiseOr());
				return getLogicalAnd(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException &ne){
			if(opFound){
				if(op){
					op->accept(this->deleteVisitor);
				}
				throw ParserException("Found '" + opName + "', but missing an argument ", currentToken.getPosition());
			}
			throw NoticeException("No logical and found");
		}
}


BasicNode* Parser::getBitwiseOr(BasicNode *left){
		FunctionCallNode *op = nullptr;	
		bool opFound = false;
		string opName = "";
		try {
			if(left == nullptr){
				left = getBitwiseXor();
			}

			if(currentToken == Token(Token::OPERATOR, "|")){
				opFound = true;
				opName = currentToken.getText();
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArgument(left);
				op->addArgument(getBitwiseXor());
				return getBitwiseOr(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException &ne){
			if(opFound){
				throw ParserException("Found '" + opName + "', but missing an argument ", currentToken.getPosition());
			}
			throw NoticeException("No bitwise or found");
		}
}

BasicNode* Parser::getBitwiseXor(BasicNode *left){
		bool opFound = false;
		string opName = "";
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getBitwiseAnd();
			}

			if(currentToken == Token(Token::OPERATOR, "^")){
				opFound = true;
				opName = currentToken.getText();
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArgument(left);
				op->addArgument(getBitwiseAnd());
				return getBitwiseXor(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException &ne){
			if(opFound){
				if(op){
					op->accept(this->deleteVisitor);
				}
				throw ParserException("Found '" + opName + "', but missing an argument ", currentToken.getPosition());
			}
			throw NoticeException("No bitwise xor found");
		}
}


BasicNode* Parser::getBitwiseAnd(BasicNode *left){
		bool opFound = false;
		string opName = "";
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getComparison();
			}

			if(currentToken == Token(Token::OPERATOR, "&")){
				opFound = true;
				opName = currentToken.getText();
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArgument(left);
				op->addArgument(getComparison());
				return getBitwiseAnd(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException &ne){	
			if(opFound){
				if(op){
					op->accept(this->deleteVisitor);
				}
				throw ParserException("Found '" + opName + "', but missing an argument ", currentToken.getPosition());
			}
			throw NoticeException("No bitwise and found");
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
		bool opFound = false;
		string opName = "";
		try {
			if(left == nullptr){
				left = getStrictComparison();
			}

			if(isComparisonOp()){
				opFound = true;
				opName = currentToken.getText();
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArgument(left);
				op->addArgument(getStrictComparison());
				return getComparison(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException ne){
			if(opFound){
				if(op){
					op->accept(this->deleteVisitor);
				}
				throw ParserException("Found '" + opName + "', but missing an argument ", currentToken.getPosition());
			}
			throw NoticeException("No comparison found");
		}
}



bool Parser::isStrictComparisonOp(){
	return 
			currentToken.getText() == "=="
			||
			currentToken.getText() == "!="
	;
}

BasicNode* Parser::getStrictComparison(BasicNode *left){
		bool opFound = false;
		string opName = "";
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getShift();
			}

			if(isStrictComparisonOp()){
				opFound = true;
				opName = currentToken.getText();
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArgument(left);
				op->addArgument(getShift());
				return getStrictComparison(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException ne){
			if(opFound){
				if(op){
					op->accept(this->deleteVisitor);
				}
				throw ParserException("Found '" + opName + "', but missing an argument ", currentToken.getPosition());
			}
			throw NoticeException("No strict comparison found");
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
		bool opFound = false;
		string opName = "";
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getExpr6();
			}

			if(isShiftOp()){
				opFound = true;
				opName = currentToken.getText();
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArgument(left);
				op->addArgument(getExpr6());
				return getShift(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException ne){
			if(opFound){
				if(op){
					op->accept(this->deleteVisitor);
				}
				throw ParserException("Found '" + opName + "', but missing an argument ", currentToken.getPosition());
			}
			throw NoticeException("No shift found");
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
		bool opFound = false;
		string opName = "";
		FunctionCallNode *op = nullptr;	
		try {
			if(left == nullptr){
				left = getExpr7();
			}

			if(isExpr6Op()){
				opFound = true;
				opName = currentToken.getText();
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArgument(left);
				op->addArgument(getExpr7());
				return getExpr6(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException ne){
			if(opFound){
				if(op){
					op->accept(this->deleteVisitor);
				}
				throw ParserException("Found '" + opName + "', but missing an argument ", currentToken.getPosition());
			}
			throw NoticeException("No EXPR6 found");
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
		bool opFound = false;
		string opName = "";
		try {
			if(left == nullptr){
				left = getExpr8();
			}

			if(isExpr7Op()){
				opFound = true;
				opName = currentToken.getText();
				op = new FunctionCallNode(currentToken);
				get(Token::OPERATOR);
				op->addArgument(left);
				op->addArgument(getExpr8());
				return getExpr7(op);
			}
			else{
				return left;
			}
		}
		catch (NoticeException ne){
			if(opFound){
				if(op){
					op->accept(this->deleteVisitor);
				}
				throw ParserException("Found '" + opName + "', but missing an argument ", currentToken.getPosition());
			}
			throw NoticeException("No EXPR7 found");
		}
}


bool Parser::isExpr8Op(){
	return currentToken.getText() == "**";
}

BasicNode* Parser::getExpr8(){
	BasicNode *left = nullptr;
	FunctionCallNode *op = nullptr;
	bool opFound = false;
	string opName = "";
	try{
		left = getExpr9();
		if(isExpr8Op()){
			opFound = true;
			opName = currentToken.getText();
			op = new FunctionCallNode(currentToken);
			get(Token::OPERATOR);
			op->addArgument(left);
			op->addArgument(getExpr8());
			return op;
		}
		else{
			return left;
		}
	}
	catch(NoticeException ne){
		if(opFound){
			if(op){
				op->accept(this->deleteVisitor);
			}
			throw ParserException("Found '" + opName + "', but missing an argument ", currentToken.getPosition());
		}
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

	try{
		return getValue();
	}
	catch(NoticeException &ne){

	}

	throw NoticeException("No ATOM found");

}

BasicNode* Parser::getName(){
	CompoundNameNode *result = nullptr;
	if(currentToken.typeEqualsTo(Token::NAME)){
		result = new CompoundNameNode(currentToken.getText());
		consume();
		return result;
	}

	throw NoticeException("No NAME");
}

BasicNode* Parser::getFunctionCalls(BasicNode *left){

		bool commaFound = false;
		FunctionCallNode *result = nullptr;
		if(currentToken.typeEqualsTo(Token::BRACE_LEFT)){
					get(Token::BRACE_LEFT);
					try{	
							result = new FunctionCallNode(left);
							result->setPosition(currentToken.getPosition());
							result->addArgument(getAssignment());
							while(currentToken == Token(Token::OPERATOR, ",")){
								get(Token::OPERATOR);
								commaFound = true;
								result->addArgument(getAssignment());
								commaFound = false;
							}
					}
					catch (NoticeException  &ne){
						if(commaFound){
							if(result){
								result->accept(this->deleteVisitor);
							}
							throw ParserException("Missing argument after comma at " + currentToken.getPosition().toString());
						}
					};
					if(!currentToken.typeEqualsTo(Token::BRACE_RIGHT)){
						if(result){
							result->accept(this->deleteVisitor);
						}
						throw ParserException("Missed ')' at " + currentToken.getPosition().toString());
					}
					get(Token::BRACE_RIGHT);

					return getFunctionCalls(result);
		}
	
		return left;
}

BasicNode* Parser::getAccessArgs(){
	try {
		return getExpression();
	}
	catch (NoticeException ne){
		throw ParserException("Empty expression is not allowed in '[]'", currentToken.getPosition());
	}

	throw NoticeException("No  '[EXPRESION]' found!");
}

BasicNode* Parser::getAccesses(BasicNode *left){

		FunctionCallNode *result = nullptr;
		if(currentToken.typeEqualsTo(Token::BRACKET_LEFT)){
			try {
					get(Token::BRACKET_LEFT);
					result = new FunctionCallNode(Token(Token::OPERATOR, "[]"));
					result->setPosition(currentToken.getPosition());
					result->addArgument(left);
					result->addArgument(getAccessArgs());
					if(!currentToken.typeEqualsTo(Token::BRACKET_RIGHT)){
						throw ParserException("Missed BRACKET_RIGHT at " + currentToken.getPosition().toString());
					}
					get(Token::BRACKET_RIGHT);

					return getAccesses(result);
			}
			catch(NoticeException ne){
				if(result){
					result->accept(this->deleteVisitor);
				}

				if(left){
					left->accept(this->deleteVisitor);
				}	
		
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

	try{
		if(left == nullptr){
			left = getExpr10();
		}
	}
	catch (NoticeException &ne){
		throw NoticeException("No EXPR10 found at "+ currentToken.getPosition().toString());
	}

	if(isExpr9SuffixOp()){
		auto tmpToken = Token(Token::OPERATOR, "_" + currentToken.getText());
		op = new FunctionCallNode(tmpToken);
		op->setPosition(currentToken.getPosition());
		get(Token::OPERATOR);	
		op->addArgument(getExpr9Suffix(left));
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
	string opName = "";

	if(isExpr9Op()){
		op = new FunctionCallNode(currentToken);
		opName = currentToken.getText();
		get(Token::OPERATOR);
		try{
			op->addArgument(getExpr9());
			return op;
		}
		catch (NoticeException &ne){
			op->accept(this->deleteVisitor);
			throw ParserException("Corrupted expression: " + opName, currentToken.getPosition());
		}
	}

	
		return getExpr9Suffix();

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
	DotNode *op = nullptr;

	bool opFound = false;
	string opName = "";


	if(left == nullptr){
		try{
			left = getAtom();
			left = getBraced(left);
				if(isExpr10Op()){
					op = new DotNode(currentToken);
					opName = currentToken.getText();
					get(Token::OPERATOR);
					opFound = true;
					right = getAtom();
					op->setLeft(left);
					op->setRight(right);
					return getExpr10(op);
				}
				else {

					return left;
				}
			}
			catch(NoticeException &ne){
				if(op){
					op->accept(this->deleteVisitor);
				}
				if(left){
					left->accept(this->deleteVisitor);
				}	
				if(currentToken.typeEqualsTo(Token::BRACKET_LEFT)){
					throw ParserException("Strange '['", currentToken.getPosition());
				}

				if(currentToken.typeEqualsTo(Token::BRACKET_RIGHT)){
					throw ParserException("Strange ']'", currentToken.getPosition());
				}
				if(opFound){
					throw ParserException("Found " + opName + ", but missing an argument " + currentToken.getPosition().toString());
				}

			}

	}
	else {				
			try{
				left = getBraced(left);

				if(isExpr10Op()){
					op = new DotNode(currentToken);
					get(Token::OPERATOR);
					right = getAtom();
					op->setLeft(left);
					op->setRight(right);
					return getExpr10(op);
				}
				else {
					return getBraced(left);
				}
			}
			catch(NoticeException &ne){
				return getAtom();
			}
			return left;
	}

	if(op){
		op->accept(this->deleteVisitor);
	}
	if(left){
		left->accept(this->deleteVisitor);
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
		tmp->setPosition(currentToken.getPosition());
		consume();
		return tmp;
	}

	throw NoticeException("No VALUE found!");
}

BasicNode* Parser::getExpression(){

	return getCommaExpression();
}

CompoundNameNode* Parser::getCompoundName(){

	int counter = 0;
	bool dotFound = false;

	CompoundNameNode *result = new CompoundNameNode();
	result->setPosition(currentToken.getPosition());
	do {
		if(counter > 0){
			dotFound = true;
			get(Token::OPERATOR);
		}

		if(currentToken == Token(Token::KEYWORD, "operator")){
			get(Token::KEYWORD);
			if(!currentToken.typeEqualsTo(Token::OPERATOR)){
				throw ParserException("Corrupted 'operator' signature (missing OPERATOR name)", currentToken.getPosition());
			}
			result->addName(currentToken);
			get(Token::OPERATOR);
		}
		else
		if(currentToken.typeEqualsTo(Token::NAME)){
			result->addName(currentToken.getText());
			get(Token::NAME);
			dotFound = false;
		}
		else {
			if(dotFound){
				if(result){
					result->accept(this->deleteVisitor);
				}	
				throw ParserException("Found '.', but missing an name in compound name", currentToken.getPosition());
			}
			throw NoticeException("No compound name found!");
		}
		++counter;
	}
	while(currentToken == Token(Token::OPERATOR, "."));

	return result;
	
}

bool Parser::isStorageMode(){
	return
		currentToken == Token(Token::KEYWORD, "volatile")
		||
		currentToken == Token(Token::KEYWORD, "static")
		||
		currentToken == Token(Token::KEYWORD, "registre")
		||
		currentToken == Token(Token::KEYWORD, "extern")
		||
		currentToken == Token(Token::KEYWORD, "const")
	;
}

bool Parser::isModifier(){
	return
		currentToken == Token(Token::KEYWORD, "long")
		||
		currentToken == Token(Token::KEYWORD, "short")
		||
		currentToken == Token(Token::KEYWORD, "signed")
		||
		currentToken == Token(Token::KEYWORD, "unsigned")
	;
}

bool Parser::isAccessMode(){
	return
		currentToken == Token(Token::OPERATOR, "@")
		||
		currentToken == Token(Token::KEYWORD, "ref")
	;
}


BasicNode* Parser::getType(){

	TypeNode *result = new TypeNode();
	result->setPosition(currentToken.getPosition());
	try{
		while(isStorageMode()){
			result->addStorageMode(currentToken.getText());
			consume();
		}

		while(isModifier()){
			result->addModifier(currentToken.getText());
			consume();
		}

		while(isAccessMode()){
			result->addAccessMode(currentToken.getText());
			consume();
		}

		result->addName(dynamic_cast<CompoundNameNode*>(getCompoundName()));

		if(currentToken.typeEqualsTo(Token::BRACKET_LEFT)){
			while(currentToken.typeEqualsTo(Token::BRACKET_LEFT)){
				get(Token::BRACKET_LEFT);
				if(!currentToken.typeEqualsTo(Token::BRACKET_RIGHT)){
					result->addDimension(dynamic_cast<ValueNode*>(getValue()));
					if(!currentToken.typeEqualsTo(Token::BRACKET_RIGHT)){
						throw ParserException("Missing ']' in type ", currentToken.getPosition());
					}
				}
				else {
					result->addDimension(new ValueNode(Token::INT, "0"));
				}
				get(Token::BRACKET_RIGHT);
			}
		}

		return result;
	}
	catch (NoticeException &ne){

		
			
		if(result->getStorageModes().size() > 0){
			result->accept(this->deleteVisitor);
			throw ParserException("Only Storage Modes, no Type name specified! ", currentToken.getPosition());
		}

		if(result->getModifiers().size() > 0){
			result->accept(this->deleteVisitor);
			throw ParserException("Only Modifiers, no Type name specified! ", currentToken.getPosition());
		}

		if(result->getAccessModes().size() > 0){
			result->accept(this->deleteVisitor);
			throw ParserException("Only Access Modes, no Type name specified! ", currentToken.getPosition());
		}

		throw NoticeException("Not TYPE found!");
	}
	
}

BasicNode* Parser::getVarDeclaration(){
	VarDeclarationNode *result = new VarDeclarationNode(currentToken.getPosition());

	try{
		result->setType(dynamic_cast<TypeNode*>(getType()));

		int count = 0;
		do{	
			if(count > 0){
				get(Token::OPERATOR);
			}

			auto variable = dynamic_cast<CompoundNameNode*>(getCompoundName());
			BasicNode *value = nullptr;
			if(currentToken == Token(Token::OPERATOR, "=")){
				get(Token::OPERATOR);
				value = getAssignment();
			}
			result->addVariable(make_pair(variable, value));
			++count;
		}
		while(currentToken == Token(Token::OPERATOR, ","));

		return result;
	}
	catch(NoticeException &ne){
		throw NoticeException("No Var Declaration found (" + ne.what() + ")!");
	}
}

BasicNode* Parser::getSignature(){
	if(currentToken != Token(Token::KEYWORD, "def")){
		throw ParserException("I shouldn't have got here!", currentToken.getPosition());
	}
	get(Token(Token::KEYWORD, "def"));

	bool typeFound = false;
	bool nameFound = false;
	bool eqFound = false;

	SignatureNode *result = new SignatureNode(currentToken.getPosition());
	TypeNode *type = nullptr;
	CompoundNameNode *name = nullptr;
	BasicNode *defaultValue = nullptr;

	try {
		if(currentToken == Token(Token::OPERATOR, "~")){
			get(Token::OPERATOR);
			auto typeTmp = new TypeNode();
			auto typeNameTmp = new CompoundNameNode(currentToken.getPosition());
			typeNameTmp->addName("void");
			typeTmp->addName(typeNameTmp);
			result->setType(typeTmp);

			TypeNode *destroyType = dynamic_cast<TypeNode*>(getType());
			//auto tmpName = dynamic_cast<CompoundNameNode*>(getCompoundName());
			auto tmpName = destroyType->getName();
			string lastName = tmpName->getSimpleName();
			lastName = destroyType->getMangledQualifiers() + lastName;
			tmpName->popName();
			lastName += "#destruct";
			tmpName->addName(lastName);
			result->setName(tmpName);
			delete destroyType;
			result->setDestruct();
		}
		else{
			result->setType(dynamic_cast<TypeNode*>(getType()));
			typeFound = true;
			if(!currentToken.typeEqualsTo(Token::NAME) && currentToken != Token(Token::KEYWORD, "operator")){
				auto tmp = new CompoundNameNode(*result->getType()->getName());
				string lastName = tmp->getSimpleName();
				lastName = result->getType()->getMangledQualifiers() + lastName;
				tmp->popName();
				lastName += "#construct";
				tmp->addName(lastName);
				result->setName(tmp);
				result->setConstruct();
			}
			else{
				result->setName(dynamic_cast<CompoundNameNode*>(getCompoundName()));
				nameFound = true;
			}
		}
		if(!currentToken.typeEqualsTo(Token::BRACE_LEFT)){
			throw ParserException("No '(' in function signature", currentToken.getPosition());
		}
		get(Token::BRACE_LEFT);

		bool commaFound = false;
		if(currentToken != Token(Token::NAME, "void")){
			try{
				int counter = 0;
				do{
					if(counter > 0){
						commaFound = true;
						get(Token::OPERATOR);
					}

					type = dynamic_cast<TypeNode*>(getType());
					name = nullptr;
					defaultValue = nullptr;
					if(currentToken != Token(Token::OPERATOR, "...") 
						&&
						currentToken != Token(Token::OPERATOR, ",") 
						&& 
						!currentToken.typeEqualsTo(Token::BRACE_RIGHT)
					){
						name = dynamic_cast<CompoundNameNode*>(getName());
						defaultValue = nullptr;
						if(currentToken == Token(Token::OPERATOR, "=")){
							eqFound = true;
							get(Token::OPERATOR);
							defaultValue = getTernary();
						}
					}

					result->addArgument(make_tuple(type, name, defaultValue));
					++counter;
					commaFound = false;
					eqFound = false;
				}
				while(currentToken == Token(Token::OPERATOR, ","));

				if(currentToken == Token(Token::OPERATOR, "...")){
					result->setVarargs();
					get(Token::OPERATOR);
				}

			}
			catch(NoticeException &ne){
				if(eqFound){
					result->accept(this->deleteVisitor);
					throw ParserException("In function signature got '=', but no defaultValue ", currentToken.getPosition());
				}

				if(commaFound){
					result->accept(this->deleteVisitor);
					throw ParserException("In function signature got ',', but no argument", currentToken.getPosition());
				}

			}
		}
		else{
			get(Token::KEYWORD);
		}

		if(!currentToken.typeEqualsTo(Token::BRACE_RIGHT)){
			if(result){
				result->accept(this->deleteVisitor);
			}
			throw ParserException("No ')' in function signature", currentToken.getPosition());
		}
		get(Token::BRACE_RIGHT);

		return result;

	}
	catch (NoticeException &ne){
		// if(result){
		// 	result->accept(this->deleteVisitor);
		// }
		if(typeFound && !nameFound){
			result->accept(this->deleteVisitor);
			throw ParserException("In function signature got type, but no name ", currentToken.getPosition());
		}
	}

	throw NoticeException("No function signature found!");
}

BasicNode* Parser::getBlock(){

	if(!currentToken.typeEqualsTo(Token::CURL_LEFT)){
		throw NoticeException("No block found!");
	}
	get(Token::CURL_LEFT);

	BlockNode *result =  new BlockNode(currentToken.getPosition());
	result->addChild(getOperators());

	if(!currentToken.typeEqualsTo(Token::CURL_RIGHT)){
		result->accept(this->deleteVisitor);
		throw ParserException("Missing '}' in block" + currentToken.getPosition().toString());
	}
	get(Token::CURL_RIGHT);

	return result;
}

BasicNode* Parser::getFunction(){
	FunctionDefinitionNode *result = nullptr;
	try {
		auto signature = dynamic_cast<SignatureNode*>(getSignature());
		
		if(!currentToken.typeEqualsTo(Token::CURL_LEFT)){
			return signature;
		}

		result = new FunctionDefinitionNode(currentToken.getPosition());
		result->setSignature(signature);
		if(currentToken == Token(Token::CURL_LEFT)){
			get(Token::CURL_LEFT);
			result->setOperators(dynamic_cast<OperatorsNode*> (getOperators()));
			if(currentToken != Token(Token::CURL_RIGHT)){
				throw ParserException("Function without '}'!", currentToken.getPosition());
			}
			get(Token::CURL_RIGHT);
		}
		return result;
	}
	catch (NoticeException &ne){
		if(result){
			result->accept(this->deleteVisitor);
		}
		throw NoticeException("No function definition found!");
	}
}

BasicNode* Parser::getWhile(){
	get(Token(Token::KEYWORD, "while"));

	bool conditionFound = false;
	bool loopFound = false;

	if(!currentToken.typeEqualsTo(Token::BRACE_LEFT)){
		throw ParserException("Missing '(' for 'while'", currentToken.getPosition());
	}
	get(Token::BRACE_LEFT);

	WhileNode *result = new WhileNode(currentToken.getPosition());

	try{
		result->setCondition(getExpression());
		conditionFound = true;

		if(!currentToken.typeEqualsTo(Token::BRACE_RIGHT)){
			throw ParserException("Missing ')' for 'while'", currentToken.getPosition());
		}
		get(Token::BRACE_RIGHT);

		result->setLoop(getOperator());
		loopFound = true;

		if(currentToken == Token(Token::KEYWORD, "else")){
			get(Token::KEYWORD);
			result->setElseBranch(getOperator());
		}

		return result;
	}
	catch (NoticeException &ne){

		if(loopFound){
			//result->accept(this->deleteVisitor);
			throw ParserException("Corrupted 'else' branch in 'while' ", currentToken.getPosition());
		}

		if(conditionFound){
			//result->accept(this->deleteVisitor);
			throw ParserException("Corrupted 'loop' in 'while' (if you want an empty operator, put ';')", currentToken.getPosition());
		}

		throw ParserException("Empty expressions are not allowed in 'while'", currentToken.getPosition());
	}		
}

BasicNode* Parser::getFor(){
	get(Token(Token::KEYWORD, "for"));

	bool initFound = false;
	bool condFound = false;
	bool stepFound = false;

	if(!currentToken.typeEqualsTo(Token::BRACE_LEFT)){
		throw ParserException("Missing '(' for 'for'", currentToken.getPosition());
	}
	get(Token::BRACE_LEFT);

	ForNode *result = new ForNode(currentToken.getPosition());
	try{
		try {
			lock();
			result->setInit(getVarDeclaration());
		}
		catch (NoticeException &ne){
			try {
				recoil();
				result->setInit(getExpression());
			}
			catch (NoticeException &ne){
				throw ParserException("Missing init in 'for'", currentToken.getPosition());
			}
		}
		initFound = true;

		get(Token::SEMICOLON);
		result->setCondition(getExpression());
		condFound = true;


		get(Token::SEMICOLON);
		result->setStep(getExpression());
		stepFound = true;

		if(!currentToken.typeEqualsTo(Token::BRACE_RIGHT)){
			throw ParserException("Missing ')' for 'for'", currentToken.getPosition());
		}
		get(Token::BRACE_RIGHT);


		result->setAction(getOperator());

		return result;
	}	
	catch (NoticeException &ne){
		if(stepFound){
			throw ParserException("No 'action' found in 'for'", currentToken.getPosition());
		}

		if(condFound){
			throw ParserException("No step found in 'for'", currentToken.getPosition());
		}

		if(initFound){
			throw ParserException("No condition found in 'for'", currentToken.getPosition());
		}

		throw ParserException("Something strange happened in 'for'", currentToken.getPosition());
	}
}

BasicNode* Parser::getIf(){
	get(Token(Token::KEYWORD, "if"));

	bool conditionFound = false;
	bool thenBranchFound = false;

	if(!currentToken.typeEqualsTo(Token::BRACE_LEFT)){
		throw ParserException("Missing '(' for 'if'", currentToken.getPosition());
	}
	get(Token::BRACE_LEFT);

	IfNode *result = new IfNode(currentToken.getPosition());

	try{
		result->setCondition(getExpression());
		conditionFound = true;

		if(!currentToken.typeEqualsTo(Token::BRACE_RIGHT)){
			throw ParserException("Missing ')' for 'if'", currentToken.getPosition());
		}
		get(Token::BRACE_RIGHT);

		result->setThenBranch(getOperator());
		thenBranchFound = true;

		if(currentToken == Token(Token::KEYWORD, "else")){
			get(Token::KEYWORD);
			result->setElseBranch(getOperator());
		}

		return result;
	}
	catch (NoticeException &ne){
		if(thenBranchFound){
			throw ParserException("Corrupted 'else' branch in 'if' ", currentToken.getPosition());
		}

		if(conditionFound){
			throw ParserException("Corrupted 'then' branch in 'if' (if you want an empty operator, put ';')", currentToken.getPosition());
		}

		throw ParserException("Empty expressions are not allowed in 'if'", currentToken.getPosition());
	}
}

bool Parser::isReturnKeyword(){
	return 
		currentToken == Token(Token::KEYWORD, "return")
		||
		currentToken == Token(Token::KEYWORD, "yield")
	;
}

BasicNode* Parser::getReturn(){
	if(!isReturnKeyword()){
		throw ParserException("I shouldn't have got to here!");
	}

	ReturnNode *result = new ReturnNode(currentToken.getPosition());

	result->setName(currentToken.getText());
	get(Token::KEYWORD);

	try {
		result->setResult(getExpression());
	}
	catch(NoticeException &ne){
	}

	return result;
}


BasicNode* Parser::getStruct(){
	if(currentToken != Token(Token::KEYWORD, "struct")){
		throw ParserException("I shouldn't have got to here!");
	}

	get(Token::KEYWORD);

	StructNode *result = new StructNode(currentToken.getPosition());

	try{
		result->setName(getCompoundName());
	}
	catch (NoticeException &ne){
		throw ParserException("No name for 'struct' found ", currentToken.getPosition());
	}

	if(!currentToken.typeEqualsTo(Token::CURL_LEFT)){
		return result;
	}

	get(Token::CURL_LEFT);
	try{
		while(true){
			consumeSemicolons();

			if(currentToken == Token(Token::KEYWORD, "def")){
				auto tmp = getFunction();
				consumeSemicolons();
				result->addFunction(tmp);
			}
			else 
			if(currentToken == Token(Token::KEYWORD, "struct")){
				auto tmp = getStruct();
				consumeSemicolons();
				result->addStructure(tmp);
			}
			else {
				auto tmp = getVarDeclaration();
				consumeSemicolons();
				result->addVariable(tmp);
			}
		}
	}
	catch(NoticeException &ne){
	}

	if(!currentToken.typeEqualsTo(Token::CURL_RIGHT)){
		throw ParserException("In 'struct' got no ')'", currentToken.getPosition());
	}
	get(Token::CURL_RIGHT);

	return result;
}

BasicNode* Parser::getImport(){

	if(currentToken != Token(Token::KEYWORD, "import")){
		throw ParserException("I shouldn't have got to here!");
	}

	auto *result = new ImportNode(currentToken.getPosition());	
	bool moduleNameFound = false;
	get(Token::KEYWORD);
	try{
		result->setModuleName(getCompoundName());
		moduleNameFound = true;
		if(currentToken == Token(Token::NAME, "to")){
			get(Token::NAME);
			result->setScopeName(getCompoundName());
		}
		return result;
	}
	catch(NoticeException &ne){
		if(moduleNameFound){
			throw ParserException("corrupted structure of 'import' -- strange with scopeName", currentToken.getPosition());
		}
		throw ParserException("corrupted structure of 'import'", currentToken.getPosition());
	}

}

bool Parser::consumeSemicolons(){
	bool result = false;
	while(currentToken.typeEqualsTo(Token::SEMICOLON)){
		get(Token::SEMICOLON);
		result = true;
	}
	return result;
}

BasicNode* Parser::getOperator(){

	bool semicolonFound = consumeSemicolons();

	if(currentToken == Token(Token::KEYWORD, "def")){
		auto result = getFunction();
		consumeSemicolons();
		return result;
	}

	if(currentToken == Token(Token::KEYWORD, "if")){
		auto result = getIf();
		consumeSemicolons();
		return result;
	}

	if(currentToken == Token(Token::KEYWORD, "while")){
		auto result = getWhile();
		consumeSemicolons();
		return result;
	}

	if(currentToken == Token(Token::KEYWORD, "for")){
		auto result = getFor();
		consumeSemicolons();
		return result;
	}

	if(currentToken == Token(Token::KEYWORD, "struct")){
		auto result = getStruct();
		consumeSemicolons();
		return result;
	}

	if(currentToken == Token(Token::KEYWORD, "import")){
		auto result = getImport();
		consumeSemicolons();
		return result;
	}

	if(currentToken.typeEqualsTo(Token::CURL_LEFT)){
		auto result = getBlock();
		consumeSemicolons();
		return result;
	}

	if(isReturnKeyword()){
		auto result = getReturn();
		consumeSemicolons();
		return result;
	}


	if(currentToken.typeEqualsTo(Token::KEYWORD) && currentToken != Token(Token::KEYWORD, "else") 
		&& !isModifier() && !isStorageMode()){
		throw ParserException("Strange keyword '" + currentToken.getText() + "' ", currentToken.getPosition());
	}


	try{
		lock();
		auto result = getVarDeclaration();
		consumeSemicolons();
		return result;
	}
	catch(NoticeException &ne){
		recoil();
	}
	
	try{
		auto result = getExpression();
		consumeSemicolons();
		return result;
	}
	catch(NoticeException &ne){
		
	}

	
	
	if(semicolonFound){
		return new OperatorsNode();
	}

	throw NoticeException("Nothing found!");
}

BasicNode* Parser::getOperators(){
	OperatorsNode *result = new OperatorsNode(currentToken.getPosition());
		try{
			while(true){
				result->addOperator(getOperator());
			}
		}
		catch(NoticeException &ne){
			//if(ne.what() != "Nothing found!"){
			//	throw ne;
			//}
		}
		return result;
}

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
			this->tree->addChild(getOperators());
			//this->tree->addChild(getVarDeclaration());
		}
		catch(NoticeException &ne){
			//cout << ne.what();
		}
		catch (LexerException &le){
			throw ParserException("Error in lexer:\n" + le.what(), currentToken.getPosition());
		}

		get(Token::END);
	}
	catch(NoticeException &ne){
		throw ParserException("Program structure corrupted: " + ne.what(), currentToken.getPosition());
	}
}
