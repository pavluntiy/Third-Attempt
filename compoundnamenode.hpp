#ifndef COMPOUNDNAMENODE
#define COMPOUNDNAMENODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "token.hpp"
#include <string>

class CompoundNameNode: public BasicNode {
protected:
	//string left;
	//CompoundNameNode *right;
	vector<string> names;


public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

//	string getLeft();
//	CompoundNameNode* getRight();

	string transformOperators(const Token &token);
	vector<string>& getNames();

	void addName(string what);

	CompoundNameNode(const Token &token);
	CompoundNameNode();
	CompoundNameNode(string left);

	string getName();
	string getSimpleName();
	bool isSimpleName();

};

#endif