#ifndef SIGNATURENODE
#define SIGNATURENODE

#include "basicnode.hpp"
#include "basicvisitor.hpp"
#include "typenode.hpp"


class FunctionSymbol;
class SignatureNode: public BasicNode {
public:
	typedef tuple<TypeNode*, CompoundNameNode*, BasicNode*> ArgumentType;
	enum class SpecialType{
		NONE,
		CONSTRUCT,
		DESTRUCT
	};
protected:
	SpecialType functionType;
	TypeNode *type;
	CompoundNameNode *name;
	vector<ArgumentType> arguments;
	bool varargs = false;

	FunctionSymbol *functionSymbol;

public: 
	void accept(BasicVisitor *visitor) override;
	string toString() override;

	TypeNode* getType();
	CompoundNameNode* getName();
	vector<ArgumentType>& getArguments();
	bool getVarargs();

	void setVarargs();
	void setType(TypeNode*);
	void setFunctionSymbol(FunctionSymbol *symbol);

	void setName(CompoundNameNode*);
	void addArgument(ArgumentType);
	FunctionSymbol* getFunctionSymbol();

	SignatureNode(Position position);
	SignatureNode();

	void setConstruct();
	void setDestruct();

	bool isConstruct();
	bool isDestruct();


};

#endif