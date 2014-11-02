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

	Token tryAndGetOctal(bool zeroFound, string buffer){
		if(!zeroFound){
			return Token();
		}
		Position startPosition = data.sourcePosition;

		while(Alphabet::is<Alphabet::ZERO>(data.currentChar)){
			data.consume();
		}

		if(Alphabet::is<Alphabet::OCTAL_DIGIT>(data.currentChar)){
			while(Alphabet::is<Alphabet::DECIMAL_DIGIT>(data.currentChar)  
				|| Alphabet::is<Alphabet::UNDERSCORE>(data.currentChar)
			){
				 

				if(!Alphabet::is<Alphabet::UNDERSCORE>(data.currentChar)){
					buffer += data.currentChar;
				}
					
				data.consume();

			}
		}

		if(buffer == "0"){
			data.restore();
			return Token();
		}

		if(data.currentChar == '.'){
				 	data.restore();
				 	return Token();
		}

		return Token(Token::INT, buffer, "octal", data.previousSourcePosition);

	}

	Token tryAndGetDecimal(bool zeroFound, string buffer){



		while(Alphabet::is<Alphabet::ZERO>(data.currentChar)){
			data.consume();
		}

		bool isFloat = false;
		if(Alphabet::is<Alphabet::DECIMAL_DIGIT>(data.currentChar) || data.currentChar == '.'){
			while(Alphabet::is<Alphabet::DECIMAL_DIGIT>(data.currentChar) 
				|| (data.currentChar == '.' && !data.find("..") )
				|| Alphabet::is<Alphabet::DECIMAL_DIGIT>(data.currentChar)
				|| Alphabet::is<Alphabet::UNDERSCORE>(data.currentChar)
			){	

				if(data.currentChar == '.'){
					if(isFloat){
						throw LexerException("FLOAT with excessive dot!");
					}
					isFloat = true;
				}

				if(!Alphabet::is<Alphabet::UNDERSCORE>(data.currentChar)){
					buffer += data.currentChar;
				}
					
				data.consume();

			}
		}

		if (isFloat){
			return Token(Token::FLOAT, buffer, "decimal", data.previousSourcePosition);
		}

		return Token(Token::INT, buffer, "decimal", data.previousSourcePosition);

	}

	Token tryAndGetNumeric(){
		data.setBackup();
		Token result;
		bool zeroFound = false;

		string buffer = "";
		if(Alphabet::is<Alphabet::SIGN>(data.currentChar)){
			buffer += data.currentChar;
			data.consume();
		}

		if (Alphabet::is<Alphabet::ZERO>(data.currentChar)){
			zeroFound = true;
			buffer += data.currentChar;
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
		while(Alphabet::is<Alphabet::WHITESPACE>(data.currentChar) 
				|| Alphabet::is<Alphabet::NEWLINE>(data.currentChar) 
			){
				data.consume();
			}
	}


	Token getToken(){
			Token result;
			try{
				data.isReady();
			}
			catch (DataException de){
				this->wasEof = true;
				throw LexerException("Eof already occured!");
			}

			getWhitespaces();
			
			if(Alphabet::is<Alphabet::NUMBER_CHAR>(data.currentChar)){
				result = tryAndGetNumeric();
				if(!result.typeEqulasTo(Token::NONE)){
					return result;
				}
			}



			if(data.eof()){
				this->wasEof = true;
				return Token();
			}
			throw LexerException("Nothing recognized!");
			return Token();
	}

	bool eof(){
		return this->wasEof;
	}

	void recover(string message = ""){



		if(message != ""){
			addToOutput(Token(Token::ERROR, message));

		}

		data.restorePosition(lastSuccessTokenEnd);
		cout << "Restored!: ";
		cout << data.currentChar << '\n';

	}

	void addToOutput (const Token &token){
		this->output.push_back(token);
		this->size++;
		this->lastSuccessTokenEnd = data.getPosition() - 1;
		cout << "Added: " << data.getSize() << data.getSourcePosition().toString() << '\n';
	}

	void getNextToken(){
		if (!data.eof()){	
			cout << "Data isnt eof\n";
				try {
					currentToken = getToken();
				}
				catch (LexerException le) {
					recover();
					return;
				}

				if(!currentToken.typeEqulasTo(Token::NONE)){
					addToOutput(currentToken);
				}	
		}
		else{
			cout << "Hurrah!\n";
			addToOutput(Token (Token::END, data.sourcePosition));
			this->wasEof = true;
		}

	}

	Token tokenAt(int index){
		
		if(eof() && index >= this->size){
			throw LexerException("Invalid index of token");
		}
		
		while (!eof() && index >= this->size){
			getNextToken();
		}	
		cout << "Trying to get token at " << index << "\n";

		if(eof() && index >= this->size){
			throw LexerException("Invalid index of token");
		}
		

		return output[index];
	}

	Token operator[](int index){
		return this->tokenAt(index);
	}

};

#endif