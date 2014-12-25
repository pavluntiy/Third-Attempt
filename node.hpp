#ifndef NODE
#define NODE

#include "header.hpp"
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

	vector<Node* >& getChildren();

	Node (Type type, string text = "", string note = "");

	Node (const Node &other);

	bool operator == (const Node &other);

	bool typeEqualsTo(const Node &other);

	string getText();


	string typeToText();
	void addChild(Node *what);

	~Node();
};

#endif