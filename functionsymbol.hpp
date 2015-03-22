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
vector<Type*> argumentTypes;
FunctionType *functionType;
AbstractScope *functionScope;
string functionLabel;
Position definitionPosition;
bool defined;
bool varargs = false;
public:

	void setFunctionType(FunctionType *type);

	void addArgumentType(Type *type);
	void setFunctionScope(AbstractScope *scope);
	void setFunctionLabel(string str);
	void setReturnType(Type *type);

	FunctionType* getFunctionType();
	Type* getReturnType();
	vector<Type*> getArgumentTypes();
	AbstractScope* getFunctionScope();
	string getFunctionLabel();

	bool isOnlyDeclared();
	void define(Position position = {0, 0});

	Position getDefinitionPosition();
	void setDefinitionPosition(Position position);

	string toString(string shift = "");
	string argumentsToString(string shift = "");

	bool exactlyEquals(FunctionSymbol *);
	bool isVarargs();
	void setVarargs();

	FunctionSymbol();


};

#endif