#ifndef NODE
#define NODE

#include "token.h"
#include "header.h"
#include <iostream>
class Node {
public:

	enum  Type {
		PROGRAM,
		BEGIN,
		END, 

		EXPRESSION, 



		EXPR1, 
		EXPR2, 
		EXPR3, 
		EXPR4, 
		EXPR5,
		EXPR6,
		EXPR7,
		EXPR8,
		EXPR9,
		EXPR10,

		BRACED,

		L_OR,
		L_AND,
		B_OR,
		B_XOR,
		B_AND,

		OPERATOR,
		OPERATORS,
	
		INT, 
		FLOAT, 
		CHAR, 
		BOOL, 
		STRING,
		NAME, 
		VALUE, 
		CONSTANT, 
		ID,

		FUNCCALL,
		FUNCARGS,
		ACCESS,
		ACCESSARGS,

		TYPE,
		TYPE_NAME,
		TYPE_MOD,
		POINTER_MOD,
		VARDECL,
		VARDECL_LIST,
		VARDECL_ELEM,

		FUNC_SIGN,
		SIGN_ARGS,
		ARGS,
		TYPE_LIST,
		ARG,
		FUNC_DEF,

		IF,
		// THEN,
		// ELSE,

		WHILE,
		DOWHILE,
		FOR,
		RETURN,

		BLOCK
				
	};

protected:

string text, note;
Type type;
vector<Node* > children;
Position position;
public:	

	vector<Node* >& getChildren(){
		return this->children;
	}

	Node (Type type, string text = "", string note = ""){
		this->type = type;
		this->text = text;
		this->note = note;
	}

	Node (const Node &other){
		this->type = other.type;
		this->text = other.text;
		this->note = other.note;
	}

	bool operator == (const Node &other){
		return this->type == other.type && this->text == other.text;
	}

	bool typeEqualsTo(const Node &other){
		return this->type == other.type;
	}

	string getText(){
		return this->text;
	}


	string typeToText(){
		switch (this->type){
			case Type::PROGRAM : return "PROGRAM";
			case Type::BEGIN 	: return "BEGIN";
			case Type::END : return "END"; 

			case  Type::EXPRESSION : return "EXPRESSION";


			case Type::EXPR1 : return "EXPR1";
			case Type::EXPR2 : return "EXPR2"; 
			case Type::EXPR3 : return "EXPR3"; 
			case Type::EXPR4 : return "EXPR3"; 
			case Type::EXPR5 : return "EXPR5";
			case Type::EXPR6 : return "EXPR6"; 
			case Type::EXPR7 : return "EXPR7"; 
			case Type::EXPR8 : return "EXPR8";
			case Type::EXPR9 : return "EXPR9";
			case Type::EXPR10 : return "EXPR10";

			case Type::BRACED : return "BRACED";

			case Type::L_OR : return "L_OR";
			case Type::L_AND: return "L_AND";
			case Type::B_OR : return "B_OR";
			case Type::B_XOR: return "B_XOR";
			case Type::B_AND: return "B_AND";

			case Type::OPERATOR: return "OPERATOR";
			case Type::OPERATORS: return "OPERATORS";
		
			case Type::INT: return "INT"; 
			case Type::FLOAT: return "FLOAT"; 
			case Type::CHAR: return "CHAR"; 
			case Type::BOOL: return "BOOL"; 
			case Type::STRING: return "STRING";
			case Type::NAME: return "NAME"; 
			case Type::VALUE: return "VALUE"; 
			case Type::CONSTANT: return "CONSTANT";
			case Type::ID: return "ID";
			case Type::FUNCCALL: return "FUNCCALL";

			case Type::FUNCARGS: return "FUNCARGS";
			case Type::ACCESS: return "ACCESS";
			case Type::ACCESSARGS: return "ACCESSARGS";

			case Type::TYPE: return "TYPE";
			case Type::TYPE_NAME: return "TYPE_NAME";
			case Type::TYPE_MOD: return "TYPE_MOD";
			case Type::POINTER_MOD: return "POINTER_MOD";
			case Type::VARDECL: return "VARDECL";
			case Type::VARDECL_LIST: return "VARDECL_LIST";
			case Type::VARDECL_ELEM: return "VARDECL_ELEM";

			case Type::FUNC_SIGN: return "FUNC_SIGN";
			case Type::SIGN_ARGS: return "SIGN_ARGS";
			case Type::ARGS: return "ARGS";
			case Type::TYPE_LIST: return "TYPE_LIST";
			case Type::ARG: return "ARG";
			case Type::FUNC_DEF: return "FUNC_DEF";

			case Type::IF: return "IF";
			// case Type::THEN: return "THEN";
			// case Type::ELSE: return "ELSE";
			case Type::WHILE: return "WHILE";
			case Type::DOWHILE: return "DOWHILE";
			case Type::FOR: return "FOR";
			case Type::RETURN: return "RETURN";

			case Type::BLOCK: return "BLOCK";



		}
		return "NONE";
	}

	void addChild(Node *what){
		this->children.push_back(what);
	}

};

#endif