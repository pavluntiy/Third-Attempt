#ifndef COMPOUNDNAMENODE
#define COMPOUNDNAMENODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "token.hpp"
#include <string>

class BasicSymbol;
class CompoundNameNode: public BasicNode {
protected:
	//string left;
	//CompoundNameNode *right;
	vector<string> names;
	BasicSymbol *symbol;


public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

//	string getLeft();
//	CompoundNameNode* getRight();

	string transformOperators(const Token &token);
	vector<string>& getNames();

	void addName(string what);

	CompoundNameNode(const Token &token);
	CompoundNameNode(Position position);
	CompoundNameNode();
	CompoundNameNode(string left);
	

	string getName();
	string getSimpleName();
	bool isSimpleName();
	BasicSymbol* getSymbol();
	void setSymbol(BasicSymbol *symbol);
};

#endif