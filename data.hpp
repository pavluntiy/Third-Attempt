
#ifndef DATA 
#define DATA

#include "dataexception.hpp"
#include "alphabet.hpp"
#include "header.hpp"

class Data{

	istream &in;
	string str;
	//bool wasEof;
	bool ready;
	bool errorOccured;

	
	int spacesInTab;
	string badCharacters;


public:

	char currentChar;
	Position sourcePosition;
	int currentPosition;
	stack<Position> previousSourcePosition;
	stack<int> previousPosition;
	Data(istream &in = cin);
	bool wasError();

	char pop();

	void push(char c);

	void recover ();
	
	int getSize();

	string getErrorReport();

	bool eof();

	Position getSourcePosition();

	int getPosition ();

	char charAt(int index);

	char operator [] (int index);

	void consume(int n);

	void consumeNewlineTabs();

	void makeReady();

	void pushCharToStream(char c = '\n');

	void isReady();

	void lock();

	void restore();

	Position getPreviousSourcePosition();

	string dataDump();

	void consume();

	bool get(std::string text);

	bool get(char c);

	bool find(char c);

	bool find(std::string text);
};

#endif