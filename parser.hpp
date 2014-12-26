#ifndef PARSER
#define PARSER

#include "token.hpp"
#include "node.hpp"
#include "parserexception.hpp"
#include "noticeexception.hpp"
#include "lexer.hpp"

#include "programnode.hpp"
#include "expressionnode.hpp"
#include "compoundnamenode.hpp"
#include "functioncallnode.hpp"
#include "valuenode.hpp"

#include <vector>
#include <set>
#include <stack>
#include <map>


class Parser{

protected: 
	BasicNode *tree;
	Lexer &lexer;

	stack<int> history;
	map<pair<Node::Type, int>, pair<bool, int> > memo;
	int currentPosition;
	Token currentToken;
	// TreeVisitor &visitor;



public:
	Parser (Lexer &lexer);

void lock();
void recoil();
void memoize(Node::Type type, int position, bool success, int jump);
void consume();

void get(Token token);

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

// Node *getFUNCARGS();

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

// Node *getNON_EMPTY_EXPRESSION();

BasicNode *getExpression();

// Node *getTYPE_MOD();

// Node *getPOINTER_MOD();

// Node *getTYPENAME_OP();

// Node *getCOMPOUND_NAME(Node *left = nullptr);

// Node *getTYPE();

// Node *getVARDECL_ELEM();

// Node *getVARDECL();

// Node *getARG();

// Node *getFUNC_SIGN_ARGS();

// Node *getFUNC_SIGN();

// Node *getOPERATOR();

// Node *getOPERATORS();

// Node *getBLOCK();

// Node *getFUNC_DEF();

// Node *getIF();

// Node *getWHILE();

// Node *getDOWHILE();
// Node *getFOR();

// bool is_RETURN_keyword();

// Node *getRETURN();

// Node *getSPECIAL();

	
BasicNode *getTree();

void pushTree();

void buildTree();
};


#endif