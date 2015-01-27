#ifndef FUNCTIONSCOPE
#define FUNCTIONSCOPE

#include "header.hpp"
#include "basicscope.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"

class FunctionScope: public BasicScope {

string name;
Type *returnType;
public:

	void setReturnType(Type *type);
	//virtual void declareFunction(FunctionSymbol *function) override;
	//virtual void declareVariable(VariableSymbol *variable) override;
	virtual void declareType(Type *type) override;
	virtual void declareStructure(StructureSymbol *structure) override;
	//virtual void declareNamedScope(AbstractScope *scope) override;

	FunctionScope();
	FunctionScope(AbstractScope *scope, string name = "");
	virtual void dump(ostream *out, string shift = "") override;
	virtual string toString(string shift = "") override;

};

#endif