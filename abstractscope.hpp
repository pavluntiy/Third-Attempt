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
class FunctionCallSymbol;


class AbstractScope {
protected:
AbstractScope *parentScope;
string name;
map<string, AbstractScope*> namedScopes;
vector<AbstractScope*> anonymousScopes;
//map<string, Type*> types;
map<string, vector<Type*> > types;
map<string, vector<FunctionSymbol*>> functions;
//map<string, FunctionSymbol*> functions;
map<string, VariableSymbol*> variables;
map<string, StructureSymbol*> structures;
map<string, BasicNode*> modules;
map<Type*, StructureSymbol*> typeStructures;

string base;
int currentOffset;

public:
	virtual AbstractScope* resolveNamedScope(const vector<string>& longName) = 0;
	virtual AbstractScope* resolveNamedScope(CompoundNameNode*) = 0;
	virtual FunctionSymbol* resolveFunction(CompoundNameNode*) = 0;
	virtual VariableSymbol* resolveVariable(CompoundNameNode*) = 0;
	virtual bool hasType(Type*) = 0;
	virtual Type* resolveModifiedType(const Type&) = 0;
	virtual Type* resolveType(CompoundNameNode*) = 0;
	virtual Type* resolveType(Type*) = 0;
	virtual Type* resolveType(const Type&) = 0;
	virtual Type* resolveType(string) = 0;
	//virtual FunctionSymbol* resolveFunction(CompoundNameNode* function) = 0;
	virtual FunctionSymbol* resolveFunction(FunctionSymbol* function) = 0;
	virtual FunctionSymbol* resolveFunctionCall(FunctionCallSymbol*) = 0;
	virtual Type* getUnqualifiedType(string name) = 0;
	virtual StructureSymbol* resolveStructure(const string &name) = 0;
	virtual StructureSymbol* resolveStructure(Type *) = 0;

	virtual BasicSymbol* resolve(string name) = 0;
	virtual BasicSymbol* resolve(CompoundNameNode *name) = 0;


	virtual map<string, vector<Type*>>& getTypes() = 0;
	//virtual map<string, Type*>& getTypes() = 0;
	//virtual map<string, FunctionSymbol*>& getFunctions() = 0;
	virtual map<string, vector<FunctionSymbol*>>& getFunctions() = 0;
	virtual map<string, VariableSymbol*>& getVariables() = 0;
	virtual map<string, AbstractScope*>& getNamedScopes() = 0;
	virtual vector<FunctionSymbol*> getOverloadedFunctionList(CompoundNameNode *) = 0;
	virtual vector<AbstractScope*>& getAnonymousScopes() = 0;
	virtual map<string, StructureSymbol*>& getStructures() = 0;
	virtual map<string, BasicNode*>& getModules() = 0;

	virtual map<Type*, StructureSymbol*>& getTypeStructures() = 0;


	virtual void addModule(BasicNode *tree, string name) = 0;
	virtual BasicNode* resolveModule(string name) = 0;
	virtual bool hasModule(string name) = 0;
	virtual void declareFunction(FunctionSymbol *function) = 0;
	virtual void declareVariable(VariableSymbol *variable) = 0;
	virtual void declareType(Type *type) = 0;
	virtual void declareType(string name, Type *type) = 0;
	virtual void declareStructure(StructureSymbol *structure) = 0;
	virtual void declareNamedScope(AbstractScope *scope) = 0;

	virtual void declareAnonymousScope(AbstractScope *scope) = 0;

	virtual string getName() = 0;
	virtual void setName(string str) = 0;

	virtual void setParentScope(AbstractScope *parentScope) = 0;
	virtual AbstractScope* getParentScope() = 0;

	virtual void dump(ostream *out, string shift = "") = 0;
	virtual void dumpFunctions(ostream *out, string shift = "") = 0;
	virtual void dumpVariables(ostream *out, string shift = "") = 0;
	virtual void dumpStructures(ostream *out, string shift = "") = 0;
	virtual void dumpTypes(ostream *out, string shift = "") = 0;
	virtual bool isDefined(string name) = 0 ;
	virtual bool isFunction(string name) = 0;
	virtual bool isVariable(string name) = 0;
	virtual bool isStructure(string name) = 0;
	virtual bool isNamedScope(string name) = 0;


	virtual void addFunction(string name, FunctionSymbol *function) = 0;
	virtual void addVariable(string name, VariableSymbol *variable) = 0;
	virtual void addType(string name, Type *type) = 0;
	virtual void addNamedScope(string name, AbstractScope *scope) = 0;
	virtual void addStructure(string name, StructureSymbol *structure) = 0;
	virtual void addTypeStructure(Type *type, StructureSymbol *structure) = 0;

	virtual void import(AbstractScope*) = 0;


};

#endif