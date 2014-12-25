#ifndef TOKEN
#define TOKEN

#include "token.hpp"



	Token::Token(Type type = Type::NONE, string text = "", string note = "", Position position = {0, 0}){
			this->type = type;
			this->text = text;
			this->note = note;
			this->position = position;
		}

	Token::Token(Type type, Position position){
			this->type = type;
			this->text = "";
			this->note = "";
			this->position = position;
		}
	

	string Token::typeToString()
	const{
		switch (this->type){
			case Type::NEWLINE: return "NEWLINE"; break;
			case Type::BEGIN: return "BEGIN"; break;
			case Type::END: return "END"; break;
			case Type::NONE: return "NONE"; break;
			case Type::INT: return "INT"; break;
			case Type::FLOAT: return "FLOAT"; break;
			case Type::CHAR: return "CHAR"; break;
			case Type::STRING: return "STRING"; break;
			case Type::OPERATOR: return "OPERATOR"; break;
			case Type::ERROR: return "ERROR"; break;
			case Type::NAME: return "NAME"; break;
			case Type::KEYWORD: return "KEYWORD"; break;
			case Type::SEMICOLON: return "SEMICOLON"; break;
			case Type::DIRECTIVE: return "DIRECTIVE"; break;
			case Type::COMMENT: return "COMMENT"; break;
			case Type::BRACKET_LEFT: return "BRACKET_LEFT"; break;
			case Type::BRACKET_RIGHT: return "BRACKET_RIGHT"; break;
			case Type::CURL_LEFT: return "CURL_LEFT"; break;
			case Type::CURL_RIGHT: return "CURL_RIGHT"; break;
			case Type::BRACE_LEFT: return "BRACE_LEFT"; break;
			case Type::BRACE_RIGHT: return "BRACE_RIGHT"; break;
		}
		return "NONE";
	}

	bool Token::typeEqualsTo(Type type)
	const{
		return this->type == type;
	}

	bool operator Token::== (const Token &other){
		return this->typeEqualsTo(other.getType()) && this->text == other.getText();
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

	Type Token::getType()
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

};
 ostream& operator Token::<< (ostream &cout, const Token &token){
 	if(!token.isInvisible()){
 		cout << token.toString() << '\n';
 	}
 	return cout;
 }
