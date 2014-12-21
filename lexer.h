#ifndef LEXER
#define LEXER

#include "token.h"
#include "lexer_exception.h"
#include "notice_exception.h"
#include "data.h"
#include "alphabet.h"
#include <vector>

class Lexer {
public:
		
	bool wasEof;
	Data data;
	vector<Token> output;
	Token currentToken;
	int size;
	int lastSuccessTokenEnd = 0;

	bool eofReported = false;

	bool EofReported(){
		return this->eofReported;
	}

	Lexer(istream &in = cin):
	data(in)
	{	
		this->wasEof = false;
		this->size = 1;
		output.push_back(Token(Token::BEGIN));
		currentToken = output[0];
	}

	// Lexer(Data &&data):
	// data(move(data))
	// {
	// 	this->wasEof = false;
	// 	this->size = 1;
	// 	output.push_back(Token(Token::BEGIN));
	// 	currentToken = output[0];
	// }

	char currentChar(){
		if(this->data.eof()){
			setEof();
			return '\n';
		}
		return this->data.currentChar;
	}

	void setEof(){
		this->wasEof = true;
//		this->addToOutput(Token (Token::END, data.sourcePosition));
	}

	bool closed(){
		
		return eof() && EofReported();

	}

	string getSuffix(){

			string buffer = "";
			buffer += currentChar();
			data.consume();

			while(
				Alphabet::is<Alphabet::LATIN_LETTER>(currentChar()) ||
				Alphabet::is<Alphabet::UNDERSCORE>(currentChar()) ||
				Alphabet::is<Alphabet::DECIMAL_DIGIT>(currentChar())
			){
				buffer += currentChar();
				data.consume();
			}
			return " ::: " + buffer;
	}

	Token tryAndGetOctal(bool zeroFound, string buffer){
		if(!zeroFound){
			data.restore();
			throw LexerException("No octal found!");
		}

		while(Alphabet::is<Alphabet::ZERO>(currentChar())){
			data.consume();
		}

		if(Alphabet::is<Alphabet::OCTAL_DIGIT>(currentChar())){
			while(Alphabet::is<Alphabet::OCTAL_DIGIT>(currentChar())  
				|| Alphabet::is<Alphabet::UNDERSCORE>(currentChar())
			){
				 

				if(!Alphabet::is<Alphabet::UNDERSCORE>(currentChar())){
					buffer += currentChar();
				}
				
				data.consume();

			}
		}


		if(Alphabet::is<Alphabet::DECIMAL_DIGIT>(currentChar()) || Alphabet::is<Alphabet::HEXADECIMAL_DIGIT>(currentChar())){
			throw LexerException("Nonoctal digits in octal number!");
		}

		if(buffer == "0"){
			data.restore();
			throw NoticeException("No octal found!");
		}

		if(currentChar() == '.'){
				 	throw LexerException("No fractions are allowed for octal");
		}


		// if(!Alphabet::is<Alphabet::WHITESPACE>(currentChar())){
		// 	throw LexerException("Strange thing!", MyException::Type::NOTICE);
		// }
		if(Alphabet::is<Alphabet::LETTER>(currentChar())){
			buffer += getSuffix();		
		}

		return Token(Token::INT, buffer, "octal", data.getPreviousSourcePosition());

	}

	Token tryAndGetHexadecimal(string buffer){

		int zeroes = 0;
		while(Alphabet::is<Alphabet::ZERO>(currentChar())){
			++zeroes;
			data.consume();
		}

		if(Alphabet::is<Alphabet::HEXADECIMAL_DIGIT>(currentChar())){
			while(Alphabet::is<Alphabet::HEXADECIMAL_DIGIT>(currentChar())  
				|| Alphabet::is<Alphabet::UNDERSCORE>(currentChar())
			){
				 

				if(!Alphabet::is<Alphabet::UNDERSCORE>(currentChar())){
					buffer += currentChar();
				}
				
				data.consume();

			}
		}

		if(buffer == "0x" || buffer == "0X"){
			if(!zeroes){
				data.restore();
				throw LexerException("Unfinished hexadecimal found!");
			}
			else{
				buffer += '0';
			}
		}



		if(currentChar() == '.'){
				 	data.restore();
				 	throw LexerException("No fractions are allowed for hexadecimal");
		}


		// if(!Alphabet::is<Alphabet::WHITESPACE>(currentChar())){
		// 	throw LexerException("Strange thing!", MyException::Type::NOTICE);
		// }
		if(Alphabet::is<Alphabet::LETTER>(currentChar())){
			buffer += getSuffix();		
		}

		return Token(Token::INT, buffer, "hexadecimal", data.getPreviousSourcePosition());

	}

	Token tryAndGetDecimal(string buffer){



		while(Alphabet::is<Alphabet::ZERO>(currentChar())){
	//		cout << "#3\n";
			data.consume();
		}

	//	cout << "Current char == " << data.currentChar << " ********\n";

		bool isFloat = false;
		if(Alphabet::is<Alphabet::DECIMAL_DIGIT>(currentChar()) || currentChar() == '.'){
			while(Alphabet::is<Alphabet::DECIMAL_DIGIT>(currentChar()) 
				|| (currentChar() == '.' && !data.find("..") )
				|| Alphabet::is<Alphabet::DECIMAL_DIGIT>(currentChar())
				|| Alphabet::is<Alphabet::UNDERSCORE>(currentChar())
			){	

				if(currentChar() == '.'){
					if(isFloat){
					//	data.restore();
						throw LexerException("FLOAT with excessive dot!");
					}
					isFloat = true;
				}

				if(!Alphabet::is<Alphabet::UNDERSCORE>(currentChar())){
					buffer += currentChar();
				}

		//		cout << "#4\n";
				data.consume();

			}
		}

		if (isFloat){
			return Token(Token::FLOAT, buffer, "decimal", data.getPreviousSourcePosition());
		}

		if(buffer.size() == 1 && Alphabet::is<Alphabet::SIGN>(buffer[0])){
			throw NoticeException("Orphan sign!");
		}

		// if(!Alphabet::is<Alphabet::WHITESPACE>(currentChar())){
		// 	throw LexerException("Strange thing!", MyException::Type::NOTICE);
		// }
		if(Alphabet::is<Alphabet::LETTER>(currentChar())){
			buffer += getSuffix();		
		}

	//	cout << "---> " << data.currentPosition  << " :|: " << data.currentChar << '\n';

		return Token(Token::INT, buffer, "decimal", data.getPreviousSourcePosition());

	}

	Token tryAndGetBinary(string buffer){

		int zeroes = 0;
		while(Alphabet::is<Alphabet::ZERO>(currentChar())){
			++zeroes;
			data.consume();
		}

		if(Alphabet::is<Alphabet::BINARY_DIGIT>(currentChar())){
			while(Alphabet::is<Alphabet::BINARY_DIGIT>(currentChar())  
				|| Alphabet::is<Alphabet::UNDERSCORE>(currentChar())
			){
				 

				if(!Alphabet::is<Alphabet::UNDERSCORE>(currentChar())){
					buffer += currentChar();
				}
				
				data.consume();

			}
		}

		if(buffer == "0b" || buffer == "0B"){
			if(!zeroes){
				data.restore();
				throw LexerException("No binary found!");
			}
			else{
				buffer += '0';
			}
		}



		if(currentChar() == '.'){
				 	data.restore();
				 	throw LexerException("No fractions are allowed for binary");
		}


		// if(!Alphabet::is<Alphabet::WHITESPACE>(currentChar())){
		// 	throw LexerException("Strange thing!", MyException::Type::NOTICE);
		// }
		if(Alphabet::is<Alphabet::LETTER>(currentChar())){
			buffer += getSuffix();		
		}

		return Token(Token::INT, buffer, "binar", data.getPreviousSourcePosition());

	}


	Token tryAndGetNumeric(){
		data.lock();
		Token result;
		bool zeroFound = false;

		string buffer = "";
		// if(Alphabet::is<Alphabet::SIGN>(currentChar())){
		// 	buffer += currentChar();
		// 	data.consume();
		// }

		if (Alphabet::is<Alphabet::ZERO>(currentChar())){
			zeroFound = true;
			buffer += currentChar();
			data.consume();
		}


		if(zeroFound && (currentChar() == 'x' || currentChar() == 'X')){
			buffer += currentChar();
			data.consume();
			try{
				result = tryAndGetHexadecimal(buffer);
				return result;
			}
			catch(NoticeException le){
				data.restore();
			}
		}

		if(zeroFound && (currentChar() == 'b' || currentChar() == 'B')){
			buffer += currentChar();
			data.consume();
			try{
				result = tryAndGetBinary(buffer);
				return result;
			}
			catch(NoticeException le){
				data.restore();
			}
		}
	
		if(zeroFound){
			try{
				result = tryAndGetOctal(zeroFound, buffer);
				return result;
			}
			catch(NoticeException le){
			//	cout << "asdfasdfasdfasfasdf\n";
			//	data.restore();
			}

		}	
		
		try{
		//	cout << ">>>>>>>>>>>>>>>>>>>>>> " << data.currentPosition << '\n';
			result = tryAndGetDecimal(buffer);
			return result;
		}
		catch(NoticeException le){
			data.restore();
		}

		throw NoticeException ("No numeric found!");
		return Token(Token::NONE);
	}

	Token tryAndGetChar(){

		string buffer = "";
		buffer += currentChar();
		data.consume();

		bool screenCharFound = false;
		while(currentChar() != '\'' || screenCharFound){
			screenCharFound = (currentChar() == '\\') && !screenCharFound;
			buffer += currentChar();
			data.consume();
		}
		buffer += '\'';
		data.consume();

		if(Alphabet::is<Alphabet::LETTER>(currentChar())){
			buffer += getSuffix();		
		}


		return Token(Token::CHAR, buffer, "", data.getPreviousSourcePosition());
	}

	Token tryAndGetString(){

		// if(currentChar() != '\"'){
		// 	throw NoticeException("No '\"' found!");
		// }

		string buffer = "";
		buffer += currentChar();
		data.consume();

		bool screenCharFound = false;
		while(currentChar() != '\"' || screenCharFound){
			screenCharFound = (currentChar() == '\\') && !screenCharFound;
			buffer += currentChar();
			data.consume();
		}
		buffer += '\"';
		data.consume();

		if(Alphabet::is<Alphabet::LETTER>(currentChar())){
			buffer += getSuffix();		
		}

		return Token(Token::STRING, buffer, "", data.getPreviousSourcePosition());
	}

	void getWhitespaces(){
		while(Alphabet::is<Alphabet::WHITESPACE>(currentChar()) && !Alphabet::is<Alphabet::NEWLINE>(currentChar())) {
				data.consume();
			}
	}

	Token getNewLine(){
		while(Alphabet::is<Alphabet::NEWLINE>(currentChar())){
			data.consume();
		}
		return Token(Token::NEWLINE, "", "invisible", data.getPreviousSourcePosition());
	}

	Token tryAndGetWord(){
		if(!Alphabet::is<Alphabet::LETTER>(currentChar())){
			throw NoticeException("No letter found!");
		}
		string buffer = "";

		while(Alphabet::is<Alphabet::LETTER>(currentChar()) || Alphabet::is<Alphabet::DECIMAL_DIGIT>(currentChar())){
			buffer += currentChar();
			data.consume();
		}


		return Token(Token::NAME, buffer, "", data.getPreviousSourcePosition());
	}

	Token tryAndGetOneLineComment (){
		// if(!data.find("//")){
		// 	throw NoticeException("No '//' found!");
		// }

		string buffer = "//";
		data.get("//");

		while(!Alphabet::is<Alphabet::NEWLINE>(currentChar())){
			buffer += currentChar();
			data.consume();
		}


		return Token(Token::COMMENT, buffer, "", data.getPreviousSourcePosition());
	}

	Token tryAndGetMultyLineComment (){
		// if(!data.find("/*")){
		// 	throw NoticeException("No '/*' found!");
		// }

		string buffer = "/*";
		data.get("/*");

		while(!data.find("*/")){
			buffer += currentChar();
			data.consume();
		}

		buffer += "*/";
		data.get("*/");


		return Token(Token::COMMENT, buffer, "", data.getPreviousSourcePosition());
	}

	Token tryAndGetDirective(){
		// if(currentChar() != '#'){
		// 	throw NoticeException("No '#' found!");
		// }

		std::string buffer = "#";
		data.consume();
		while (!Alphabet::is<Alphabet::NEWLINE>(currentChar()) && !data.find("//") && !data.find("/*")){
			buffer += currentChar();
			data.consume();
		}
		return Token(Token::DIRECTIVE, buffer, "", data.getPreviousSourcePosition());
	}


	Token getSlashVariants(){
			// if(currentChar() != '/'){
			// 	throw NoticeException("No '/' found!");
			// }
			data.consume();
			if (currentChar() == '/'){
				data.consume();
				return Token(Token::OPERATOR, "//", "", data.getPreviousSourcePosition());
			}
			else if (currentChar() == '='){
				data.consume();
				return Token(Token::OPERATOR, "/=", "", data.getPreviousSourcePosition());
			}
			else {
				return Token(Token::OPERATOR, "/", "", data.getPreviousSourcePosition());
			}
	}

	Token getPlusVariants (){
		// if(currentChar() != '+'){
		// 	throw NoticeException("No '*' found!");
		// }
		data.consume();
		if ( currentChar() == '+' ){
				data.consume();
				return Token(Token::OPERATOR, "++", "", data.getPreviousSourcePosition()); 
		}
		else if ( currentChar() == '=' ){
				data.consume();
				return Token(Token::OPERATOR, "+=", "", data.getPreviousSourcePosition());
		}
		else {
			return Token(Token::OPERATOR, "+", "", data.getPreviousSourcePosition());
		}
	}

	Token getMinusVariants (){
		// if(currentChar() != '-'){
		// 	throw NoticeException("No '-' found!");
		// }
		data.consume();
		if ( currentChar() == '-' ){
				data.consume();
				return Token(Token::OPERATOR, "--", "", data.getPreviousSourcePosition()); 
		}
		else if ( currentChar() == '=' ){
				data.consume();
				return Token(Token::OPERATOR, "-=", "", data.getPreviousSourcePosition());
		}
		else if (currentChar() == '>'){
			data.consume();
			return Token(Token::OPERATOR, "->", "", data.getPreviousSourcePosition());
		}
		else {
			return Token(Token::OPERATOR, "-", "", data.getPreviousSourcePosition());
		}
	}

	Token getStarVariants(){
		// if(currentChar() != '*'){
		// 	throw NoticeException("No '*' found!");
		// }
		data.consume();
		if ( currentChar() == '*' ){
			data.consume();
			return Token(Token::OPERATOR, "**", "", data.getPreviousSourcePosition()); 
		}
		else if ( currentChar() == '=' ){
			data.consume();
			return Token(Token::OPERATOR, "*=", "", data.getPreviousSourcePosition()); 
		}
		else {
			return Token(Token::OPERATOR, "*", "", data.getPreviousSourcePosition()); 
		}

	}

	Token getProcentVariants(){

		// if(currentChar() != '%'){
		// 		throw NoticeException("No '%' found!");
		// 	}
			data.consume();
			if (currentChar() == '='){
				data.consume();
				return Token(Token::OPERATOR, "%=", "", data.getPreviousSourcePosition());
			}
			else {
				return Token(Token::OPERATOR, "%", "", data.getPreviousSourcePosition());
			}
	}

	Token getCircumflexVariants(){
		// if(currentChar() != '^'){
		// 		throw NoticeException("No '^' found!");
		// 	}
			data.consume();
			if (currentChar() == '='){
				data.consume();
				return Token(Token::OPERATOR, "^=", "", data.getPreviousSourcePosition());
			}
			else {
				return Token(Token::OPERATOR, "^", "", data.getPreviousSourcePosition());
			}
	}

	Token getWaveVariants(){
		// if(currentChar() != '~'){
		// 		throw NoticeException("No '~' found!");
		// 	}
			data.consume();
			if (currentChar() == '='){
				data.consume();
				return Token(Token::OPERATOR, "~=", "", data.getPreviousSourcePosition());
			}
			else {
				return Token(Token::OPERATOR, "~", "", data.getPreviousSourcePosition());
			}
	}

	Token getBraceLeft(){
		data.consume();
		return Token(Token::BRACE_LEFT, data.getPreviousSourcePosition());
	}

	Token getBraceRight(){
		data.consume();
		return Token(Token::BRACE_RIGHT, data.getPreviousSourcePosition());
	}

	Token getBracketLeft(){
		data.consume();
		return Token(Token::BRACKET_LEFT, data.getPreviousSourcePosition());
	}

	Token getBracketRight(){
		data.consume();
		return Token(Token::BRACKET_RIGHT, data.getPreviousSourcePosition());
	}

	Token getCurlLeft(){
		data.consume();
		return Token(Token::CURL_LEFT, data.getPreviousSourcePosition());
	}

	Token getCurlRight(){
		data.consume();
		return Token(Token::CURL_RIGHT, data.getPreviousSourcePosition());
	}

	Token getDotVariants(){
		data.consume();
		if (currentChar() == '.'){
			data.consume();
			if(currentChar() == '.'){
				data.consume();
				return Token(Token::OPERATOR, "...", "", data.getPreviousSourcePosition());
			}
			return Token(Token::OPERATOR, "..", "", data.getPreviousSourcePosition());
		}
		else {
			return Token(Token::OPERATOR, ".", "", data.getPreviousSourcePosition());
		}	
	}

	Token getComaVariants(){
		data.consume();
		return Token(Token::OPERATOR, ",", "", data.getPreviousSourcePosition());	
	}

	Token  getLessVariants(){
		data.consume();
		if(currentChar() == '='){
			data.consume();
			return Token(Token::OPERATOR, "<=", "", data.getPreviousSourcePosition());
		}
		else if (currentChar() == '<'){
			data.consume();
			if(currentChar() == '='){
				data.consume();
				return Token(Token::OPERATOR, "<<=", "", data.getPreviousSourcePosition());
			}
			return Token(Token::OPERATOR, "<<", "", data.getPreviousSourcePosition());
		}
		else {
			return Token(Token::OPERATOR, "<", "", data.getPreviousSourcePosition());
		}	
	}

	Token  getGreaterVariants(){
		data.consume();
		if(currentChar() == '='){
			data.consume();
			return Token(Token::OPERATOR, ">=", "", data.getPreviousSourcePosition());
		}
		else if (currentChar() == '>'){
			data.consume();
			if(currentChar() == '>'){
				data.consume();
				return Token(Token::OPERATOR, ">>>", "", data.getPreviousSourcePosition());
			}
			if(currentChar() == '='){
				data.consume();
				return Token(Token::OPERATOR, ">>=", "", data.getPreviousSourcePosition());
			}
			return Token(Token::OPERATOR, ">>", "", data.getPreviousSourcePosition());
		}
		else {
			return Token(Token::OPERATOR, ">", "", data.getPreviousSourcePosition());
		}	
	}



	Token getEqualsVariants(){

			data.consume();
			if (currentChar() == '='){
				data.consume();
				return Token(Token::OPERATOR, "==", "", data.getPreviousSourcePosition());
			}
			else {
				return Token(Token::OPERATOR, "=", "", data.getPreviousSourcePosition());
			}
	}

	Token getAmpersandVariants(){

			data.consume();
			if (currentChar() == '&'){
				data.consume();
				return Token(Token::OPERATOR, "&&", "", data.getPreviousSourcePosition());
			}
			else if (currentChar() == '='){
				data.consume();
				return Token(Token::OPERATOR, "&=", "", data.getPreviousSourcePosition());
			}
			else {
				return Token(Token::OPERATOR, "&", "", data.getPreviousSourcePosition());
			}
	}

	Token getDashVariants(){

			data.consume();
			if (currentChar() == '|'){
				data.consume();
				return Token(Token::OPERATOR, "||", "", data.getPreviousSourcePosition());
			}
			else if (currentChar() == '='){
				data.consume();
				return Token(Token::OPERATOR, "|=", "", data.getPreviousSourcePosition());
			}
			else {
				return Token(Token::OPERATOR, "|", "", data.getPreviousSourcePosition());
			}
	}

	Token getAtVariants(){
		data.consume();
		return Token(Token::OPERATOR, "@", "", data.getPreviousSourcePosition());
	}

	Token getDollarVariants(){
		data.consume();
		return Token(Token::OPERATOR, "$", "", data.getPreviousSourcePosition());
	}


	Token getQuestionVariants(){
		data.consume();
		return Token(Token::OPERATOR, "?", "", data.getPreviousSourcePosition());
	}

	Token getColonVariants(){
		data.consume();
			if (currentChar() == ':'){
				data.consume();
				if(currentChar() == '='){
					data.consume();
					return Token(Token::OPERATOR, "::=", "", data.getPreviousSourcePosition());
				}
				return Token(Token::OPERATOR, "::", "", data.getPreviousSourcePosition());
			}
			else if (currentChar() == '='){
				data.consume();
				return Token(Token::OPERATOR, ":=", "", data.getPreviousSourcePosition());
			}
			else {
				return Token(Token::OPERATOR, ":", "", data.getPreviousSourcePosition());
			}
		
	}

	Token getExclamationVariants(){

		data.consume();
		if (currentChar() == '='){
				data.consume();
				return Token(Token::OPERATOR, "!=", "", data.getPreviousSourcePosition());
			}
			else {
				return Token(Token::OPERATOR, "!", "", data.getPreviousSourcePosition());
		}
	}







	Token tryAndGetOperator(){

		data.lock();
		switch (currentChar()){
			case '/': return getSlashVariants(); break;
			case '%': return getProcentVariants(); break;
			case '+': return getPlusVariants(); break;
			case '-': return getMinusVariants(); break;
			case '*': return getStarVariants(); break;

			case '^': return getCircumflexVariants(); break;
			case '~': return getWaveVariants(); break;

			case '(': return getBraceLeft(); break;
			case ')': return getBraceRight(); break;
			
			case '[': return getBracketLeft(); break;
			case ']': return getBracketRight(); break; 

			case '{': return getCurlLeft(); break;
			case '}': return getCurlRight(); break;

			case '.': return getDotVariants(); break;
			case ',': return getComaVariants(); break;

			case '>': return getGreaterVariants(); break;
			case '<': return getLessVariants(); break;

			case '=': return getEqualsVariants(); break;

			case '&': return getAmpersandVariants(); break;
			case '|': return getDashVariants(); break;

			case '@': return getAtVariants(); break;
			case ':': return getColonVariants(); break;

			case '$': return getDollarVariants(); break;

			case '?' : return getQuestionVariants(); break;
			case '!' : return getExclamationVariants(); break;

		}

		data.restore();
		throw NoticeException("Nothing found!");


	}


	Token getToken(){
			Token result;
			try{
				data.isReady();
			}
			catch (DataException de){
				setEof();
			//	return Token(Token::END, data.sourcePosition);
				throw LexerException("Eof already occured!");
			}



			try {
				getWhitespaces();
			}
			catch (DataException de){
				setEof();
			//	cout << data.dataDump();
			//	return(Token (Token::END, data.sourcePosition));
				throw NoticeException("Eof already occured!");
			}

			if(Alphabet::is<Alphabet::NEWLINE>(currentChar())){
				try {
					result = getNewLine();
					return result;
				}
				catch(DataException de){
					setEof();
				//	cout << data.dataDump();
				//	cout << "asdfasdfasdf";
				//	return(Token (Token::END, data.sourcePosition));
					throw NoticeException("Eof already occured!");
				}
			}

			

			if(Alphabet::is<Alphabet::NUMBER_CHAR>(currentChar())){
				try{
					result = tryAndGetNumeric();
					return result;
				}
				catch (NoticeException le){
				}
			}

			if(currentChar() == '\''){
				try{
					result = tryAndGetChar();
					return result;
				}
				catch (NoticeException le){
				}
			}

			if(currentChar() == '\"'){
				try{
					result = tryAndGetString();
					return result;
				}
				catch (NoticeException le){

				}
			}

			if(Alphabet::is<Alphabet::LETTER>(currentChar())){
				try{
					result = tryAndGetWord();
					return result;
				}
				catch (NoticeException le){
				}
			}

			if(currentChar() == '#'){
				try{
					result = tryAndGetDirective();
					return result;
				}
				catch (NoticeException le){
				}
			}

			if(data.find("//")){
				try{
					result = tryAndGetOneLineComment();
					return result;
				}
				catch (NoticeException le){
				}
			}

			if(data.find("/*")){
				try{
					result = tryAndGetMultyLineComment();
					return result;
				}
				catch (NoticeException le){
				}
			}

			if(currentChar() == ';'){
				data.consume();
				return Token(Token::SEMICOLON, data.sourcePosition);
			}

			try{
				result = tryAndGetOperator();
				return result;
			}
			catch (NoticeException le){
			}

			//cout << "Nothing strange!\n\n";




			if(data.eof()){
				setEof();
			//	cout << "Returning end\n";
			//	cout << data.dataDump();
			//	cout << "opqwerqweqweq";
			//	return(Token (Token::END, data.sourcePosition));

			}
			data.restore();
			throw LexerException("Unknown characters ");
			return Token();
	}

	bool eof(){
		return this->wasEof;
	}

	void recover(){

		for(int i = this->size - 1; i > 1 && !output[i].typeEqulasTo(Token::NEWLINE); --i){
			output[i].setType(Token::NONE);
		}
		
	//	cout << "Blablab" << endl;
		data.recover();
	//	data.consume();
	//	cout << currentChar() << '\n';

	}


	void addToOutput (const Token &token){
		this->currentToken = token;
		this->output.push_back(token);
		this->size++;
	//	this->lastSuccessTokenEnd = data.getPosition() - 1;
	//	cout << "Added: " << data.getSize() << data.getSourcePosition().toString() << '\n';
	//	cout << "Last succeeded token at position " << data.getPosition() << '\n';
		data.lock();
	}

	void getNextToken(){
		//Nicify!
				try {
					currentToken = getToken();
				}
				catch (LexerException le) {
					recover();
					addToOutput(Token(Token::ERROR, le.what() + ": " + data.getErrorReport(), "", data.getPreviousSourcePosition()));
			//		addToOutput(Token(Token::ERROR, le.what()));
					return;
				}
				catch (NoticeException ne){
					return;
				}

		//		if(!currentToken.typeEqulasTo(Token::NONE)){
					
				addToOutput(currentToken);
		//		}	

	}

	void reportEof(void){
		addToOutput(Token (Token::END, data.sourcePosition));
		this->eofReported = true;
	}

	Token tokenAt(int index){
	
		if(eof() && index >= this->size){

			if(index == this->size && !EofReported()){
				reportEof();
			}
			else{
				throw LexerException("Invalid index of token");
			}
		}
		
		while (!eof() && index >= this->size){
	//	while(index >= this->size){
	//		cout << "blabla\n\n";
			getNextToken();
		}	
	//	cout << "Trying to get token at " << index << "\n";
	//	cout << currentToken;
	//	cout << "Hurrah!\n";
		// if(eof() && index >= this->size){
		// 	throw LexerException("Invalid index of token");
		// }

		// if(eof()){
		// 	addToOutput(Token (Token::END, data.sourcePosition));
		// }
		if(index == this->size && !EofReported()){
				reportEof();
			}
		

		return output[index];
	}

	Token operator[](int index){
		return this->tokenAt(index);
	}

};

#endif