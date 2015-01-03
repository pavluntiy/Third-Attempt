#ifndef TYPENODE
#define TYPENODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "compoundnamenode.hpp"
#include "valuenode.hpp"


class Type;

class TypeNode: public BasicNode {
protected:
	vector<string> storageModes;
	vector<string> modifiers;
	vector<string> accessModes;
	vector<ValueNode*> dimensions;
	CompoundNameNode *name;
//	Type *type;



public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	void addStorageMode(string what);
	void addModifier(string what);
	void addAccessMode(string what);
	void addDimension(ValueNode *what);
	void addName(CompoundNameNode *name);
//	void setTypeSymbol(Type *type);

	vector<string>& getStorageModes();
	vector<string>& getModifiers();
	vector<string>& getAccessModes();
	vector<ValueNode*>& getDimensions();
	CompoundNameNode* getName();
//	Type* getTypeSymbol();

	TypeNode(Position position);
	TypeNode();


};

#endif