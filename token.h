#ifndef TOKEN
#define TOKEN

#include <string>
#include <iostream>
#include "header.h"
using namespace std;

class Token {

public:	
	enum  Type {
		NEWLINE,
		BEGIN,
		END,
		NONE,
		INT,
		FLOAT,
		CHAR,
		STRING,
		OPERATOR,
		ERROR,
		NAME,
		KEYWORD,
		SEMICOLON,
		DIRECTIVE,
		COMMENT,
		BRACE_LEFT,
		BRACE_RIGHT,
		CURL_LEFT,
		CURL_RIGHT,
		BRACKET_LEFT,
		BRACKET_RIGHT
	};

private:	
 	string text, note;
 		 Type type; 
 		 Position position;	
public:
	Token(Type type = Type::NONE, string text = "", string note = "", Position position = {0, 0}){
			this->type = type;
			this->text = text;
			this->note = note;
			this->position = position;
		}

	Token(Type type, Position position){
			this->type = type;
			this->text = "";
			this->note = "";
			this->position = position;
		}
	

	string typeToString()
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
			case Type::CURL_LEFT: return "BRACKET_LEFT"; break;
			case Type::CURL_RIGHT: return "BRACKET_RIGHT"; break;
			case Type::BRACE_LEFT: return "BRACKET_LEFT"; break;
			case Type::BRACE_RIGHT: return "BRACKET_RIGHT"; break;
		}
		return "NONE";
	}

	bool typeEqulasTo(Type type){
		return this->type == type;
	}

	string toString()
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

	bool isInvisible()
	const{
		return this->note == "invisible";
	}

	void setType(Token::Type type){
		this->type = type;
	}



};
 ostream& operator << (ostream &cout, const Token &token){
 	if(!token.isInvisible()){
 		cout << token.toString() << '\n';
 	}
 	return cout;
 }

#endif