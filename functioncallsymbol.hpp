#ifndef FUNCTIONCALLSYMBOL
#define FUNCTIONCALLSYMBOL


#include "header.hpp"
#include "type.hpp"
//#include "functionscope.hpp"
#include "basicsymbol.hpp"
#include "abstractscope.hpp"
#include "functiontype.hpp"
#include "functionsymbol.hpp"

//class FunctionSymbol;
class FunctionScope;
class FunctionCallSymbol: public BasicSymbol{
vector<Type*> arguments;
FunctionSymbol *function;
vector<FunctionSymbol*> conversions;

public:
	void addArgument(Type *type);
	void setFunctionScope(AbstractScope *scope);
	void setFunctionLabel(string str);
	vector<Type*> getArguments();
	AbstractScope* getFunctionScope();
	string getFunctionLabel();

	void setFunction(FunctionSymbol *function);
	FunctionSymbol* getFunction();

	Type *getType();

	string toString(string shift = "");
	string argumentsToString(string shift = "");

	bool exactlyEquals(FunctionSymbol *);
	bool conversionExists(FunctionSymbol *);

	const vector<FunctionSymbol*>& getConversions();

	FunctionCallSymbol();


};

#endif