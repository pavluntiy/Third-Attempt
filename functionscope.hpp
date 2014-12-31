#ifndef FUNCTIONSCOPE
#define FUNCTIONSCOPE

#include "header.hpp"
#include "basicscope.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"

class FunctionScope: public BasicScope {
public:
	virtual FunctionSymbol* resolveFunction(string name) override;
	virtual VariableSymbol* resolveVariable(string name) override;
	virtual Type* resolveType(string name) override;
	virtual Type* resolveType(Type) override;

	virtual void declareFunction(FunctionSymbol function) override;
	virtual void declareVariable(VariableSymbol variable) override;
	virtual void declareType(Type type) override;
	FunctionScope(string name);
	FunctionScope();
	virtual void dump(ostream *out) override;

};

#endif