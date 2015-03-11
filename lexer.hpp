#ifndef LEXER
#define LEXER

#include "header.hpp"
#include "token.hpp"
#include "lexerexception.hpp"
#include "noticeexception.hpp"
#include "data.hpp"
#include "alphabet.hpp"


class Lexer {
public:
		
	bool wasEof;
	Data *data;
	vector<Token> output;
	Token currentToken;
	set<string> keywords;

	bool eofReported = false;

	bool EofReported();

	Lexer(Data *data);

	char currentChar();
	void setEof();
	bool closed();

	string getSuffix();

	Token tryAndGetOctal(bool zeroFound, string buffer);

	Token tryAndGetHexadecimal(string buffer);

	Token tryAndGetDecimal(string buffer);
	Token tryAndGetBinary(string buffer);

	Token tryAndGetNumeric();

	Token tryAndGetChar();

	Token tryAndGetString();

	void getWhitespaces();

	Token getNewLine();

	Token tryAndGetWord();

	Token tryAndGetOneLineComment ();

	Token tryAndGetMultyLineComment ();

	Token tryAndGetDirective();


	Token getSlashVariants();

	Token getPlusVariants ();

	Token getMinusVariants ();
	Token getStarVariants();

	Token getProcentVariants();

	Token getCircumflexVariants();

	Token getWaveVariants();

	Token getBraceLeft();

	Token getBraceRight();

	Token getBracketLeft();

	Token getBracketRight();

	Token getCurlLeft();

	Token getCurlRight();

	Token getDotVariants();

	Token getComaVariants();

	Token  getLessVariants();

	Token  getGreaterVariants();

	Token getEqualsVariants();

	Token getAmpersandVariants();

	Token getDashVariants();

	Token getAtVariants();

	Token getDollarVariants();

	Token getQuestionVariants();

	Token getColonVariants();

	Token getExclamationVariants();

	Token tryAndGetOperator();

	Token getToken();
	bool eof();
	void recover();


	void addToOutput (const Token &token);
	void getNextToken();

	void checkForEof(void);

	void reportEof(void);

	Token tokenAt(int index);

	Token operator[](int index);

	bool isValidIndex(int index);

};

#endif