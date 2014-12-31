#ifndef BASICSCOPE
#define BASICSCOPE

#include "header.hpp"
#include "noticeexception.hpp"
#include "typeexception.hpp"

class FunctionSymbol;
class VariableSymbol;
class Type;

class BasicScope {
protected:
BasicScope *parentScope;
string name;
map<string, Type> types;
//map<string, vector<FunctionSymbol>> functions;
map<string, FunctionSymbol> functions;
map<string, VariableSymbol> variables;

string base;
int currentOffset;

public:
	virtual FunctionSymbol* resolveFunction(string name) = 0;
	virtual VariableSymbol* resolveVariable(string name) = 0;
	virtual Type* resolveType(string name) = 0;
	virtual Type* resolveType(Type) = 0;

	virtual void declareFunction(FunctionSymbol function) = 0;
	virtual void declareVariable(VariableSymbol variable) = 0;
	virtual void declareType(Type type) = 0;

	// virtual string getName() = 0;
	// virtual void setName(string str) = 0;

	

	virtual void dump(ostream *out) = 0;
};

#endif