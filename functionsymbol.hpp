#ifndef FUNCTIONSYMBOL
#define FUNCTIONSYMBOL

#include "header.hpp"
#include "type.hpp"
//#include "functionscope.hpp"
#include "basicsymbol.hpp"
#include "abstractscope.hpp"
#include "functiontype.hpp"

class FunctionScope;
class FunctionSymbol: public BasicSymbol{

Type* returnType;
vector<Type*> arguments;
FunctionType *functionType;
AbstractScope *functionScope;
string functionLabel;
Position definitionPosition;
bool defined;
public:

	void setReturnType(Type *type);
	void addArgument(Type *type);
	void setFunctionScope(AbstractScope *scope);
	void setFunctionLabel(string str);

	Type* getReturnType();
	vector<Type*> getArguments();
	AbstractScope* getFunctionScope();
	string getFunctionLabel();

	bool isOnlyDeclared();
	void define(Position position = {0, 0});

	Position getDefinitionPosition();
	void setDefinitionPosition(Position position);

	string toString();
	string argumentsToString();

	FunctionSymbol();


};

#endif