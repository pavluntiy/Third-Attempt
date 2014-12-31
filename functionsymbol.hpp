#ifndef FUNCTIONSYMBOL
#define FUNCTIONSYMBOL

#include "header.hpp"
#include "type.hpp"
//#include "functionscope.hpp"
#include "basicsymbol.hpp"

class FunctionScope;
class FunctionSymbol: public BasicSymbol{

Type* returnType;
vector<Type*> arguments;
FunctionScope *functionScope;
string functionLabel;
public:

	void setReturnType(Type *type);
	void addArgument(Type *type);
	void setFunctionScope(FunctionScope *scope);
	void setFunctionLabel(string str);

	Type* getReturnType();
	vector<Type*> getArguments();
	FunctionScope* getFunctionScope();
	string getFunctionLabel();



};

#endif