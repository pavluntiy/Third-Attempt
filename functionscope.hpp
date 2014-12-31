#ifndef FUNCTIONSCOPE
#define FUNCTIONSCOPE

#include "header.hpp"
#include "basicscope.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"

class FunctionScope: public BasicScope {
public:
	virtual void declareFunction(FunctionSymbol *function) override;
	virtual void declareVariable(VariableSymbol *variable) override;
	virtual void declareType(Type *type) override;
};

#endif