#ifndef FUNCTIONSYMBOL
#define FUNCTIONSYMBOL

#include "header.hpp"
#include "type.hpp"
//#include "functionscope.hpp"
#include "basicsymbol.hpp"
#include "abstractscope.hpp"

class FunctionScope;
class FunctionSymbol: public BasicSymbol{

Type* returnType;
vector<Type*> arguments;
AbstractScope *functionScope;
string functionLabel;
public:

	void setReturnType(Type *type);
	void addArgument(Type *type);
	void setFunctionScope(AbstractScope *scope);
	void setFunctionLabel(string str);

	Type* getReturnType();
	vector<Type*> getArguments();
	AbstractScope* getFunctionScope();
	string getFunctionLabel();



};

#endif