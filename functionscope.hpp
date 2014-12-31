#ifndef FUNCTIONSCOPE
#define FUNCTIONSCOPE

#include "header.hpp"
#include "basicscope.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"

class FunctionScope: public BasicScope {

Type *returnType;
public:
	virtual FunctionSymbol* resolveFunction(string name) override;
	virtual VariableSymbol* resolveVariable(string name) override;
	virtual Type* resolveType(string name) override;
	virtual Type* resolveType(Type) override;

	void setReturnType(Type *type);

	virtual void declareFunction(FunctionSymbol function) override;
	virtual void declareVariable(VariableSymbol variable) override;
	virtual void declareType(Type type) override;
	FunctionScope(string name);
	FunctionScope();
	FunctionScope(BasicScope *scope);
	virtual void dump(ostream *out) override;

};

#endif