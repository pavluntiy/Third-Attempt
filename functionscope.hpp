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
	// virtual FunctionSymbol* resolveFunction(string name) override;
	// virtual VariableSymbol* resolveVariable(string name) override;
	// virtual Type* resolveType(string name) override;
	// virtual Type* resolveType(Type) override;

	void setReturnType(Type *type);

	virtual void declareFunction(FunctionSymbol *function) override;
	virtual void declareVariable(VariableSymbol *variable) override;
	virtual void declareType(Type *type) override;
	virtual void declareStructure(StructureSymbol *structure) override;

	// virtual map<string, Type>& getTypes() override;
	// virtual map<string, FunctionSymbol>& getFunctions() override;
	// virtual map<string, VariableSymbol>& getVariables() override;

	// void setName(string name);
	// string getName();
	// void setParentScope(AbstractScope *);
	// AbstractScope *getParentScope();
	// FunctionScope(string name);
	FunctionScope();
	FunctionScope(AbstractScope *scope, string name = "");
	virtual void dump(ostream *out, string shift = "") override;

};

#endif