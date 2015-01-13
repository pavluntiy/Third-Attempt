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
#include "structuresymbol.hpp"

class BasicScope: public AbstractScope{
public:
	virtual AbstractScope* resolveNamedScope(const vector<string>& longName);
	virtual AbstractScope* resolveNamedScope(CompoundNameNode* ) override;
	virtual FunctionSymbol* resolveFunction(CompoundNameNode *name) override;
	virtual VariableSymbol* resolveVariable(CompoundNameNode *name) override;
	virtual Type* resolveModifiedType(const Type&) override;
	virtual Type* resolveType(CompoundNameNode *name) override;
	virtual Type* resolveType(Type*) override;
	virtual Type* resolveType(const Type&) override;
	virtual StructureSymbol* resolveStructure(const string &name) override;

	virtual BasicSymbol* resolve(string name) override;
	virtual BasicSymbol* resolve(CompoundNameNode *name) override;

	virtual vector<AbstractScope*>& getAnonymousScopes() override;
	virtual map<string, AbstractScope*>& getNamedScopes()override;
	virtual map<string, Type*>& getTypes() override;
	virtual map<string, FunctionSymbol*>& getFunctions() override;
	virtual map<string, VariableSymbol*>& getVariables() override;
	virtual map<string, StructureSymbol*>& getStructures() override;



	virtual void declareFunction(FunctionSymbol *function) override;
	virtual void declareVariable(VariableSymbol *variable) override;
	virtual void declareType(Type *type) override;
	virtual void declareStructure(StructureSymbol *structure) override;
	virtual void declareNamedScope(AbstractScope *scope) override;
	virtual void declareAnonymousScope(AbstractScope *scope) override;

	virtual string getName() override;
	virtual void setName(string str) override;

	virtual void setParentScope(AbstractScope *parentScope) override;
	virtual AbstractScope* getParentScope() override;

	virtual void dump(ostream *out, string shift = "") = 0;

	BasicScope();
	BasicScope(AbstractScope *, string name = "");

	virtual bool isDefined(string name) override;
	virtual bool isFunction(string name) override;
	virtual bool isVariable(string name) override;
	virtual bool isStructure(string name) override;
	virtual bool isNamedScope(string name) override;

	void addFunction(string name, FunctionSymbol *function);
	void addVariable(string name, VariableSymbol *variable);
	void addType(string name, Type *type);
	void addNamedScope(string name, AbstractScope *scope);
	void addStructure(string name, StructureSymbol *structure);

};

#endif