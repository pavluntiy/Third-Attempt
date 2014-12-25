#ifndef PARSER
#define PARSER

#include "token.hpp"
#include "node.hpp"
#include "parserexception.hpp"
#include "noticeexception.hpp"
#include "lexer.hpp"
#include "tree_visitor.h"

#include <vector>
#include <set>
#include <stack>
#include <map>


class Parser{

protected: 
	Node *tree;
	Lexer &lexer;

	stack<int> history;
	map<pair<Node::Type, int>, pair<bool, int> > memo;
	int currentPosition;
	Token currentToken;
	TreeVisitor &visitor;



public:
	Parser (Lexer &lexer, TreeVisitor &visitor);

void lock();
void recoil();
void memoize(Node::Type type, int position, bool success, int jump);
void consume();

void get(Token token);

void get(Token::Type type);
Node *getCOMMA();

Node *getCOMMA_EXPRESSION(Node *left = nullptr);

Node *getASSIGNMENT_OP();

Node *getASSIGNMENT();

Node *getTERNARY();

Node *getIS_IN_EXPRESSION_OP();

Node *getIS_IN_EXPRESSION(Node *left = nullptr);

Node *getL_OR_OP();

Node *getL_OR(Node *left = nullptr);

Node *getL_AND_OP();

Node *getL_AND(Node *left = nullptr);

Node *getCOMPARISION_OP();

Node *getCOMPARISION(Node *left = nullptr);

Node *getDIFF_COMPARISION_OP();

Node *getDIFF_COMPARISION(Node *left = nullptr);

Node *getB_OR_OP();

Node *getB_OR(Node *left = nullptr);

Node *getB_XOR_OP();

Node *getB_XOR(Node *left = nullptr);

Node *getB_AND_OP();

Node *getB_AND(Node *left = nullptr);

Node *getSHIFT_OP();

Node *getSHIFT(Node *left = nullptr);

Node *getEXPR6_OP();

Node *getEXPR6(Node *left = nullptr);

Node *getEXPR7_OP();

Node *getEXPR7(Node *left = nullptr);

Node *getBEGIN();

Node *getEND();

Node *getINT();

Node *getFLOAT();

Node *getCHAR();

Node *getSTRING();

Node *getEXPR10_OP();

Node *getEXPR8_OP();

Node *getEXPR8();

Node *getATOM();

Node *getNAME();

Node *getFUNCARGS();

Node *getFUNCCALS(Node *left);

Node *getACCESSARGS();

Node *getACCESSES(Node *left);

Node* getBRACED(Node *left = nullptr);

Node *getEXPR9_OP_SUFFIX();

Node *getEXPR9_OP();

Node *getEXPR9_SUFFIX(Node *left = nullptr);

Node *getEXPR9();

Node *getEXPR10(Node *left = nullptr);

Node *getVALUE();

Node *getNON_EMPTY_EXPRESSION();

Node *getEXPRESSION();

Node *getTYPE_MOD();

Node *getPOINTER_MOD();

Node *getTYPENAME_OP();

Node *getCOMPOUND_NAME(Node *left = nullptr);

Node *getTYPE();

Node *getVARDECL_ELEM();

Node *getVARDECL();

Node *getARG();

Node *getFUNC_SIGN_ARGS();

Node *getFUNC_SIGN();

Node *getOPERATOR();

Node *getOPERATORS();

Node *getBLOCK();

Node *getFUNC_DEF();

Node *getIF();

Node *getWHILE();

Node *getDOWHILE();
Node *getFOR();

bool is_RETURN_keyword();

Node *getRETURN();

Node *getSPECIAL();

	
Node *getTree();

void pushTree();

void buildTree();
};


#endif