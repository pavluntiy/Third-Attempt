#ifndef SIGNATURENODE
#define SIGNATURENODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "typenode.hpp"

class SignatureNode: public BasicNode {
public:
	typedef tuple<TypeNode*, CompoundNameNode*, ValueNode*> ArgumentType;
protected:
	TypeNode *type;
	CompoundNameNode *name;
	vector<ArgumentType> arguments;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	TypeNode* getType();
	CompoundNameNode* getName();
	vector<ArgumentType>& getArguments();

	void setType(TypeNode*);
	void setName(CompoundNameNode*);
	void addArgument(ArgumentType);


};

#endif