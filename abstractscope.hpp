#ifndef ABSTRACTSCOPE
#define ABSTRACTSCOPE

#include "header.hpp"
#include "noticeexception.hpp"
#include "typeexception.hpp"
#include "compoundnamenode.hpp"

class FunctionSymbol;
class VariableSymbol;
class Type;
class StructureSymbol;


class AbstractScope {
protected:
AbstractScope *parentScope;
string name;
map<string, AbstractScope*> namedScopes;
map<string, Type*> types;
//map<string, vector<FunctionSymbol>> functions;
map<string, FunctionSymbol*> functions;
map<string, VariableSymbol*> variables;
map<string, StructureSymbol*> structures;

string base;
int currentOffset;

public:
	virtual AbstractScope* resolveNamedScope(const vector<string>& longName) = 0;
	virtual AbstractScope* resolveNamedScope(CompoundNameNode*) = 0;
	virtual FunctionSymbol* resolveFunction(CompoundNameNode*) = 0;
	virtual VariableSymbol* resolveVariable(CompoundNameNode*) = 0;
	virtual Type* resolveType(CompoundNameNode*) = 0;
	virtual Type* resolveType(Type*) = 0;
	virtual Type* resolveType(const Type&) = 0;

	virtual map<string, Type*>& getTypes() = 0;
	virtual map<string, FunctionSymbol*>& getFunctions() = 0;
	virtual map<string, VariableSymbol*>& getVariables() = 0;
	virtual map<string, AbstractScope*>& getNamedScopes() = 0;
	virtual map<string, StructureSymbol*>& getStructures() = 0;



	virtual void declareFunction(FunctionSymbol *function) = 0;
	virtual void declareVariable(VariableSymbol *variable) = 0;
	virtual void declareType(Type *type) = 0;
	virtual void declareStructure(StructureSymbol *structure) = 0;
	virtual void declareNamedScope(AbstractScope *scope) = 0;

	virtual string getName() = 0;
	virtual void setName(string str) = 0;

	virtual void setParentScope(AbstractScope *parentScope) = 0;
	virtual AbstractScope* getParentScope() = 0;

	virtual void dump(ostream *out, string shift = "") = 0;


};

#endif