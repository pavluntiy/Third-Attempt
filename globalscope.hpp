#ifndef GLOBALSCOPE
#define GLOBALSCOPE

#include "header.hpp"
#include "basicscope.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"

class GlobalScope: public BasicScope {
public:
	// virtual void declareFunction(FunctionSymbol *function) override;
	// virtual void declareVariable(VariableSymbol *variable) override;
	// virtual void declareType(Type *type) override;
	// virtual void declareStructure(StructureSymbol *structure) override;
	// virtual void declareNamedScope(AbstractScope *scope) override;

	GlobalScope();
	virtual void dump(ostream *out, string shift = "") override;
	virtual string toString(string shift = "") override;
};

#endif