#ifndef FUNCTIONTYPE
#define FUNCTIONTYPE
#include "type.hpp"
#include "signaturenode.hpp"



class FunctionType: public Type {
protected:
	Type* returnType;
	vector<Type*> arguments;
	bool varargs = false;
public:

	//string getName() const;
	int getSize() const;
	FunctionType(string name, int size = 0);
	FunctionType();
	//FunctionType(SignatureNode*);

	Type* getReturnType();
	vector<Type*> getArguments();
	void setReturnType(Type *type);
	void addArgument(Type *type);
	virtual string toString(string shift = "") override;
	string argumentsToString(string shift = "");
	void setVarargs();
	bool isVarargs();
};


#endif