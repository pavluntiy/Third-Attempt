#ifndef COMPOUNDNAMENODE
#define COMPOUNDNAMENODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
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
	vector<string>& getNames();

	void addName(string what);

	CompoundNameNode();
	CompoundNameNode(string left);

};

#endif