#ifndef BASICSCOPE
#define BASICSCOPE

#include "header.hpp"
#include "noticeexception.hpp"
#include "typeexception.hpp"

// class FunctionSymbol;
// class VariableSymbol;
// class Type;

#include "abstractscope.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"
#include "type.hpp"

class BasicScope: public AbstractScope{
protected:
AbstractScope *parentScope;
string name;
map<string, Type> types;
//map<string, vector<FunctionSymbol>> functions;
map<string, FunctionSymbol> functions;
map<string, VariableSymbol> variables;

string base;
int currentOffset;

public:
	virtual FunctionSymbol* resolveFunction(string name) override;
	virtual VariableSymbol* resolveVariable(string name) override;
	virtual Type* resolveType(string name) override;
	virtual Type* resolveType(Type) override;

	virtual map<string, Type>& getTypes() override;
	virtual map<string, FunctionSymbol>& getFunctions() override;
	virtual map<string, VariableSymbol>& getVariables() override;



	virtual void declareFunction(FunctionSymbol function) = 0;
	virtual void declareVariable(VariableSymbol variable) = 0;
	virtual void declareType(Type type) = 0;

	virtual string getName() override;
	virtual void setName(string str) override;

	virtual void setParentScope(AbstractScope *parentScope) override;
	virtual AbstractScope* getParentScope() override;

	virtual void dump(ostream *out, string shift = "") = 0;
};

#endif