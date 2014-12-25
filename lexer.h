#ifndef LEXER
#define LEXER

#include "token.hpp"
#include "lexerexception.hpp"
#include "noticeexception.hpp"
#include "data.h"
#include "alphabet.h"
#include <vector>
#include <set>

class Lexer {
public:
		
	bool wasEof;
	Data data;
	vector<Token> output;
	Token currentToken;
	set<string> keywords;

	bool eofReported = false;

	bool EofReported(){
		return this->eofReported;
	}

	Lexer(istream &in = cin):
	data(in)
	{	
		this->wasEof = false;
		output.push_back(Token(Token::BEGIN));
		currentToken = output[0];

		ifstream kw("keywords");
		string keyword;
		while(kw >> keyword){
			keywords.insert(keyword);
		}
		kw.close();
	}

	char currentChar(){
		if(this->data.eof()){
			setEof();
			return '\0';
		}
		return this->data.currentChar;
	}

	void setEof(){
		this->wasEof = true;
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

		if(Alphabet::is<Alphabet::LETTER>(currentChar())){
			buffer += getSuffix();		
		}

		return Token(Token::INT, buffer, "hexadecimal", data.getPreviousSourcePosition());

	}

	Token tryAndGetDecimal(string buffer){



		while(Alphabet::is<Alphabet::ZERO>(currentChar())){
			data.consume();
		}

		bool isFloat = false;
		if(Alphabet::is<Alphabet::DECIMAL_DIGIT>(currentChar()) || currentChar() == '.'){
			while(Alphabet::is<Alphabet::DECIMAL_DIGIT>(currentChar()) 
				|| (currentChar() == '.' && !data.find("..") )
				|| Alphabet::is<Alphabet::DECIMAL_DIGIT>(currentChar())
				|| Alphabet::is<Alphabet::UNDERSCORE>(currentChar())
			){	

				if(currentChar() == '.'){
					if(isFloat){
						throw LexerException("FLOAT with excessive dot!");
					}
					isFloat = true;
				}

				if(!Alphabet::is<Alphabet::UNDERSCORE>(currentChar())){
					buffer += currentChar();
				}

				data.consume();

			}
		}

		if (isFloat){
			return Token(Token::FLOAT, buffer, "decimal", data.getPreviousSourcePosition());
		}

		if(buffer.size() == 1 && Alphabet::is<Alphabet::SIGN>(buffer[0])){
			throw NoticeException("Orphan sign!");
		}

		if(Alphabet::is<Alphabet::LETTER>(currentChar())){
			buffer += getSuffix();		
		}

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
			catch(NoticeException le){}
		}	
		
		try{
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
		return Token(Token::NEWLINE, "", "", data.getPreviousSourcePosition());
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
		if(keywords.count(buffer)){
			return Token(Token::KEYWORD, buffer, "", data.getPreviousSourcePosition());
		}


		return Token(Token::NAME, buffer, "", data.getPreviousSourcePosition());
	}

	Token tryAndGetOneLineComment (){
		string buffer = "//";
		data.get("//");

		while(!Alphabet::is<Alphabet::NEWLINE>(currentChar())){
			buffer += currentChar();
			data.consume();
		}


		return Token(Token::COMMENT, buffer, "", data.getPreviousSourcePosition());
	}

	Token tryAndGetMultyLineComment (){
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
		std::string buffer = "#";
		data.consume();
		while (!Alphabet::is<Alphabet::NEWLINE>(currentChar()) && !data.find("//") && !data.find("/*")){
			buffer += currentChar();
			data.consume();
		}
		return Token(Token::DIRECTIVE, buffer, "", data.getPreviousSourcePosition());
	}


	Token getSlashVariants(){
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
				if(currentChar() == '='){
					data.consume();
					return Token(Token::OPERATOR, "&&=", "", data.getPreviousSourcePosition());
				}
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
				if(currentChar() == '='){

					data.consume();
					return Token(Token::OPERATOR, "||=", "", data.getPreviousSourcePosition());
				}
				return Token(Token::OPERATOR, "||", "", data.getPreviousSourcePosition());
			}
			else if (currentChar() == '='){
				//cout << data.eof() << " &&\n";
				data.consume();
				//cout << data.eof() << " &&\n";
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
				throw LexerException("Eof already occured #1!");
			}

			try {
				getWhitespaces();
			}
			catch (DataException de){
				setEof();
				throw NoticeException("Eof already occured #2!");
			}

		

			if(Alphabet::is<Alphabet::NEWLINE>(currentChar())){
				try {
					result = getNewLine();
					return result;
				}
				catch(DataException de){
					setEof();
					throw NoticeException("Eof already occured #3!");
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
			}
			data.restore();
			throw LexerException("Unknown characters ");
			return Token();
	}

	bool eof(){
		return this->wasEof;
	}

	void recover(){

		for(int i = static_cast<int>(this->output.size()) - 1; i > 1 && !output[i].typeEqualsTo(Token::NEWLINE); --i){
			output[i].setType(Token::NONE);
		}
		
		data.recover();

	}


	void addToOutput (const Token &token){
		this->currentToken = token;
		this->output.push_back(token);
		data.lock();
	}

	void getNextToken(){
				try {
					currentToken = getToken();
				}
				catch (LexerException le) {
					recover();
					addToOutput(Token(Token::ERROR, le.what() + ": " + data.getErrorReport(), "", data.getPreviousSourcePosition()));
					return;
				}
				catch (NoticeException ne){
					return;
				}
					
				addToOutput(currentToken);	

	}

	void checkForEof(void){
		this->wasEof = data.eof();
	}

	void reportEof(void){
		addToOutput(Token (Token::END, data.sourcePosition));
		this->eofReported = true;
	}

	Token tokenAt(int index){
		if(eof() && index >= static_cast<int>(this->output.size())){

			if(index == static_cast<int>(this->output.size()) && !EofReported()){
				reportEof();
			}
			else{
				throw LexerException("Invalid index of token");
			}
		}
		
		while (!closed() && index >= static_cast<int>(this->output.size())){
			getNextToken();
			checkForEof();
		}	

		if(index >= static_cast<int>(this->output.size()) && !EofReported()){
			reportEof();
		}

		//		cout << currentChar() << ' ';
		return output[index];
	}

	Token operator[](int index){
		return this->tokenAt(index);
	}

	bool isValidIndex(int index){
		return (index < static_cast<int>(this->output.size()) && index >= 0) || !eofReported;
	}

};

#endif