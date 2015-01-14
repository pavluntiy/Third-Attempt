#ifndef FUNCTIONCALLSYMBOL
#define FUNCTIONCALLSYMBOL


#include "header.hpp"
#include "type.hpp"
//#include "functionscope.hpp"
#include "basicsymbol.hpp"
#include "abstractscope.hpp"
#include "functiontype.hpp"

class FunctionScope;
class FunctionCallSymbol: public BasicSymbol{

Type* returnType;
vector<Type*> arguments;
FunctionType *functionType;
public:

	void setReturnType(Type *type);
	void addArgument(Type *type);
	void setFunctionScope(AbstractScope *scope);
	void setFunctionLabel(string str);

	Type* getReturnType();
	vector<Type*> getArguments();
	AbstractScope* getFunctionScope();
	string getFunctionLabel();

	string toString(string shift = "");
	string argumentsToString(string shift = "");

	FunctionCallSymbol();


};

#endif