#ifndef SIGNATURENODE
#define SIGNATURENODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "typenode.hpp"

class SignatureNode: public BasicNode {
public:
	typedef tuple<TypeNode*, CompoundNameNode*, BasicNode*> ArgumentType;
protected:
	TypeNode *type;
	CompoundNameNode *name;
	vector<ArgumentType> arguments;
	bool varargs = false;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	TypeNode* getType();
	CompoundNameNode* getName();
	vector<ArgumentType>& getArguments();
	bool getVarargs();

	void setVarargs();
	void setType(TypeNode*);
	void setName(CompoundNameNode*);
	void addArgument(ArgumentType);


};

#endif