#ifndef LEXER
#define LEXER

#include "token.h"
#include "lexer_exception.h"
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

	Lexer(istream &in = cin):
	data(in)
	{	
		this->wasEof = false;
		this->size = 1;
		output.push_back(Token(Token::BEGIN));
	}

	char currentChar(){
		if(this->data.eof()){
			setEof();
			return '\n';
		}
		return this->data.currentChar;
	}

	void setEof(){
		this->wasEof = true;
	}

	Token tryAndGetOctal(bool zeroFound, string buffer){
		if(!zeroFound){
			return Token();
		}
	//	Position startPosition = data.sourcePosition;

		while(Alphabet::is<Alphabet::ZERO>(currentChar())){
	//		cout << "#1\n";
			data.consume();
		}

		if(Alphabet::is<Alphabet::OCTAL_DIGIT>(currentChar())){
			while(Alphabet::is<Alphabet::DECIMAL_DIGIT>(currentChar())  
				|| Alphabet::is<Alphabet::UNDERSCORE>(currentChar())
			){
				 

				if(!Alphabet::is<Alphabet::UNDERSCORE>(currentChar())){
					buffer += currentChar();
				}
				
	//			cout << "#2\n";
				data.consume();

			}
		}

		if(buffer == "0"){
			data.restore();
			return Token();
		}

		if(currentChar() == '.'){
				 	data.restore();
				 	return Token();
		}

		return Token(Token::INT, buffer, "octal", data.previousSourcePosition);

	}

	Token tryAndGetDecimal(bool zeroFound, string buffer){



		while(Alphabet::is<Alphabet::ZERO>(currentChar())){
		//	cout << "#3\n";
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

		//		cout << "#4\n";
				data.consume();

			}
		}

		if (isFloat){
			return Token(Token::FLOAT, buffer, "decimal", data.previousSourcePosition);
		}

		return Token(Token::INT, buffer, "decimal", data.previousSourcePosition);

	}

	Token tryAndGetNumeric(){
		data.lock();
		Token result;
		bool zeroFound = false;

		string buffer = "";
		if(Alphabet::is<Alphabet::SIGN>(currentChar())){
			buffer += currentChar();
	//		cout << "#5\n";
			data.consume();
		}

		if (Alphabet::is<Alphabet::ZERO>(currentChar())){
			zeroFound = true;
			buffer += currentChar();
	//		cout << "#6\n";
			data.consume();
		}

	//	data.setBackup();

	/*	result = tryAndGetHexadecimal(zeroFound);
		if (!result.typeEqulasTo(Token::NONE)){
			return result;
		}

		numeric = tryAndGetBinar(zeroFound);
		if (!result.typeEqulasTo(Token::NONE)){
			return result;
		}
	*/
		result = tryAndGetOctal(zeroFound, buffer);
		if (!result.typeEqulasTo(Token::NONE)){
			return result;
		}
		

		result = tryAndGetDecimal(zeroFound, buffer);
		if (!result.typeEqulasTo(Token::NONE)){
			return result;
		}

		return Token(Token::NONE);
	}

	void getWhitespaces(){
		while(Alphabet::is<Alphabet::WHITESPACE>(currentChar()) 
				|| Alphabet::is<Alphabet::NEWLINE>(currentChar()) 
			){
	//			cout << "#7\n";
				data.consume();
			}
	}


	Token getToken(){
			Token result;
			try{
				data.isReady();
			}
			catch (DataException de){
				setEof();
				return Token(Token::END, data.sourcePosition);
				throw LexerException("Eof already occured!");
			}



			try {
				getWhitespaces();
			}
			catch (DataException de){
				setEof();
			//	cout << data.dataDump();
				return(Token (Token::END, data.sourcePosition));
			}
			

			if(Alphabet::is<Alphabet::NUMBER_CHAR>(currentChar())){
				result = tryAndGetNumeric();
				if(!result.typeEqulasTo(Token::NONE)){
					return result;
				}
			}

	//		cout << "Nothing strange!\n\n";



			if(data.eof()){
				setEof();
			//	cout << "Returning end\n";
			//	cout << data.dataDump();
				return(Token (Token::END, data.sourcePosition));

			}
			throw LexerException("Nothing recognized!");
			return Token();
	}

	bool eof(){
		return this->wasEof;
	}

	void recover(){

		data.recover();
	//	data.consume();
	//	cout << currentChar() << '\n';

	}

	void addToOutput (const Token &token){
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
					addToOutput(Token(Token::ERROR, data.getErrorReport(), "", data.previousSourcePosition));
			//		addToOutput(Token(Token::ERROR, le.what()));
					return;
				}

		//		if(!currentToken.typeEqulasTo(Token::NONE)){
					
				addToOutput(currentToken);
		//		}	

	}

	Token tokenAt(int index){
	
		if(eof() && index >= this->size){
			throw LexerException("Invalid index of token");
		}
		
		while (!eof() && index >= this->size){
	//		cout << "blabla\n\n";
			getNextToken();
		}	
	//	cout << "Trying to get token at " << index << "\n";

	//	cout << "Hurrah!\n";
		// if(eof() && index >= this->size){
		// 	throw LexerException("Invalid index of token");
		// }
		

		return output[index];
	}

	Token operator[](int index){
		return this->tokenAt(index);
	}

};

#endif