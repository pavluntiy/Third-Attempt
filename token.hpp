#ifndef TOKEN
#define TOKEN

#include <string>
#include <iostream>
#include "header.hpp"

class Token {

public:	
	enum  Type {
		NONE,
		NEWLINE,
		BEGIN,
		END,
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
	Token(Type type = Type::NONE, string text = "", string note = "", Position position = {0, 0});

	Token(Type type, Position position);
	

	string typeToString() const;

	bool typeEqualsTo(Type type) const;

	bool operator == (const Token &other);
	bool operator != (const Token &other);

	string toString() const;

	Position getPosition() const;

	bool isInvisible() const;

	void setType(Token::Type type);

	Type getType()const;

	string getText() const;

	bool isIgnorable();

};
 ostream& operator << (ostream &cout, const Token &token);

#endif