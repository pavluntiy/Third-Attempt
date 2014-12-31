#ifndef BASICSCOPE
#define BASICSCOPE

#include "header.hpp"
//#include "type.hpp"
// #include "functionsymbol.hpp"
// #include "variablesymbol.hpp"
#include "typeexception.hpp"

class FunctionSymbol;
class VariableSymbol;
class Type;

class BasicScope {
protected:
map<string, Type* > types;
map<string, vector<FunctionSymbol*>> functions;
map<string, VariableSymbol*> variables;


string base;
int currentOffset;

public:
	virtual void declareFunction(FunctionSymbol *function) = 0;
	virtual void declareVariable(VariableSymbol *variable) = 0;
	virtual void declareType(Type *type) = 0;
};

#endif