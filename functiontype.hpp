#ifndef FUNCTIONTYPE
#define FUNCTIONTYPE
#include "type.hpp"
#include "signaturenode.hpp"



class FunctionType: public Type {
protected:
	Type* returnType;
	vector<Type*> argumentTypes;
	bool varargs = false;
public:

	//string getName() const;
	int getSize() const;
	FunctionType(string name, int size = 0);
	FunctionType();
	//FunctionType(SignatureNode*);

	Type* getReturnType();
	vector<Type*> getArgumentTypes();
	void setReturnType(Type *type);
	void addArgumentType(Type *type);
	virtual string toString(string shift = "")override;
	string argumentsToString(string shift = "");
	void setVarargs();
	bool isVarargs();
};


#endif