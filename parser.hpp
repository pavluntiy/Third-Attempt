#ifndef PARSER
#define PARSER

#include "header.hpp"

#include "token.hpp"
#include "node.hpp"
#include "parserexception.hpp"
#include "noticeexception.hpp"
#include "lexer.hpp"

#include "programnode.hpp"
#include "operatorsnode.hpp"
#include "compoundnamenode.hpp"
#include "functioncallnode.hpp"
#include "valuenode.hpp"
#include "typenode.hpp"
#include "vardeclarationnode.hpp"
#include "signaturenode.hpp"
#include "functiondefinitionnode.hpp"
#include "ifnode.hpp"
#include "whilenode.hpp"
#include "fornode.hpp"
#include "returnnode.hpp"

class Parser{

protected: 
	BasicNode *tree;
	Lexer &lexer;

	stack<int> history;
	map<pair<Node::Type, int>, pair<bool, int> > memo;
	int currentPosition;
	Token currentToken;

public:
	Parser (Lexer &lexer);

void lock();
void recoil();
void memoize(Node::Type type, int position, bool success, int jump);
void consume();

void get(Token token);
void tryAndGet(Token::Type type);
void tryAndGet(Token token);

void get(Token::Type type);

BasicNode* getCommaExpression(BasicNode *left = nullptr);

bool isAssignmentOp();

BasicNode* getAssignment();

BasicNode* getTernary();

bool isInIsExpressionOp();

BasicNode* getInIsExpression(BasicNode *left = nullptr);

BasicNode* getLogicalOr(BasicNode *left = nullptr);

BasicNode* getLogicalAnd(BasicNode *left = nullptr);

BasicNode* getBitwiseOr(BasicNode *left = nullptr);

BasicNode* getBitwiseXor(BasicNode *left = nullptr);

BasicNode* getBitwiseAnd(BasicNode *left = nullptr);

bool isComparisonOp();

BasicNode* getComparison(BasicNode *left = nullptr);

bool isStrictComparisonOp();

BasicNode* getStrictComparison(BasicNode *left = nullptr);


bool isShiftOp();

BasicNode* getShift(BasicNode *left = nullptr);

bool isExpr6Op();
BasicNode* getExpr6(BasicNode *left = nullptr);

bool isExpr7Op();
BasicNode* getExpr7(BasicNode *left = nullptr);

bool isExpr8Op();

BasicNode* getExpr8();

BasicNode* getAtom();

BasicNode* getName();

BasicNode *getFunctionCalls(BasicNode *left = nullptr);

BasicNode *getAccessArgs();

BasicNode* getAccesses(BasicNode *left);

BasicNode* getBraced(BasicNode *left = nullptr);

bool isExpr9SuffixOp();

BasicNode* getExpr9Suffix(BasicNode *left = nullptr);

bool isExpr9Op();

BasicNode* getExpr9();

bool isExpr10Op();
BasicNode* getExpr10(BasicNode *left = nullptr);
bool isValue();
BasicNode* getValue();

BasicNode *getExpression();

bool isStorageMode();

bool isModifier();

bool isAccessMode();

BasicNode* getType();

BasicNode* getVarDeclaration();


bool consumeSemicolons();
BasicNode *getOperator();

BasicNode *getOperators();

BasicNode* getSignature();

BasicNode* getBlock();

BasicNode* getFunction();

BasicNode* getIf();

BasicNode* getWhile();

BasicNode* getFor();

bool isReturnKeyword();

BasicNode* getReturn();

CompoundNameNode* getCompoundName();
	
BasicNode *getTree();

void pushTree();

void buildTree();
};


#endif