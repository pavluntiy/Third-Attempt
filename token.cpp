
#include "token.hpp"



	Token::Token(Token::Type type, string text, string note, Position position){
			this->type = type;
			this->text = text;
			this->note = note;
			this->position = position;
		}

	Token::Token(Token::Type type, Position position){
			this->type = type;
			this->text = "";
			this->note = "";
			this->position = position;
		}
	

	string Token::typeToString()
	const{
		switch (this->type){
			case Token::Type::NEWLINE: return "NEWLINE"; break;
			case Token::Type::BEGIN: return "BEGIN"; break;
			case Token::Type::END: return "END"; break;
			case Token::Type::NONE: return "NONE"; break;
			case Token::Type::INT: return "INT"; break;
			case Token::Type::FLOAT: return "FLOAT"; break;
			case Token::Type::CHAR: return "CHAR"; break;
			case Token::Type::STRING: return "STRING"; break;
			case Token::Type::OPERATOR: return "OPERATOR"; break;
			case Token::Type::ERROR: return "ERROR"; break;
			case Token::Type::NAME: return "NAME"; break;
			case Token::Type::KEYWORD: return "KEYWORD"; break;
			case Token::Type::SEMICOLON: return "SEMICOLON"; break;
			case Token::Type::DIRECTIVE: return "DIRECTIVE"; break;
			case Token::Type::COMMENT: return "COMMENT"; break;
			case Token::Type::BRACKET_LEFT: return "BRACKET_LEFT"; break;
			case Token::Type::BRACKET_RIGHT: return "BRACKET_RIGHT"; break;
			case Token::Type::CURL_LEFT: return "CURL_LEFT"; break;
			case Token::Type::CURL_RIGHT: return "CURL_RIGHT"; break;
			case Token::Type::BRACE_LEFT: return "BRACE_LEFT"; break;
			case Token::Type::BRACE_RIGHT: return "BRACE_RIGHT"; break;
		}
		return "NONE";
	}

	bool Token::typeEqualsTo(Token::Type type)
	const{
		return this->type == type;
	}

	bool Token::operator == (const Token &other){
		return this->typeEqualsTo(other.getType()) && this->text == other.getText();
	}

	bool Token::operator != (const Token &other){
		return !((*this) == other);
	}

	string Token::toString()
	const{
		string str = this->typeToString();
		if(this->text != ""){
			str += ": " + text;
		}

		if(this->note != ""){
			str += "; (" + this->note + ")";
		}

		str += " (" + position.toString() + " )";

		return str;
	}

	Position Token::getPosition(){
		return this->position;
	}

	bool Token::isInvisible()
	const{	
		return 
		this->type == Token::NEWLINE 
		|| 
		this->type == Token::NONE
		;
	}

	void Token::setType(Token::Type type){
		this->type = type;
	}

	Token::Type Token::getType()
	const{
		return this->type;
	}

	string Token::getText()
	const{
		return this->text;
	}


	bool Token::isIgnorable(){
		return 
		this->type == Token::NEWLINE 
		|| 
		this->type == Token::NONE
		||
		this->type == Token::ERROR
		||
		this->type == Token::COMMENT
		||
		this->type == Token::DIRECTIVE
		;
	}

 ostream& operator<< (ostream &cout, const Token &token){
 	if(!token.isInvisible()){
 		cout << token.toString() << '\n';
 	}
 	return cout;
 }
