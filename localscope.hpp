#ifndef LOCALSCOPE
#define LOCALSCOPE

#include "header.hpp"
#include "basicscope.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"

class LocalScope: public BasicScope {
public:
	virtual void declareFunction(FunctionSymbol *function) override;
	//virtual void declareVariable(VariableSymbol *variable) override;
	virtual void declareType(Type *type) override;
	virtual void declareStructure(StructureSymbol *structure) override;
	virtual void declareNamedScope(AbstractScope *scope) override;

	LocalScope();
	LocalScope(AbstractScope *parentScope, string name = "");
	virtual void dump(ostream *out, string shift = "") override;
};

#endif