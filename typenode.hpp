#ifndef TYPENODE
#define TYPENODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "compoundnamenode.hpp"

class TypeNode: public BasicNode {
protected:
	vector<string> storageModes;
	vector<string> modifiers;
	vector<string> accessModes;
	CompoundNameNode *name;


public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	void addStorageMode(string what);
	void addModifier(string what);
	void addAccessMode(string what);
	void addName(CompoundNameNode *name);

	vector<string>& getStorageModes();
	vector<string>& getModifiers();
	vector<string>& getAccessModes();
	CompoundNameNode* getName();


};

#endif