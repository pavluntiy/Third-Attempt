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
	virtual Type* resolveType(CompoundNameNode *name) override;
	virtual Type* resolveType(Type*) override;
	virtual Type* resolveType(const Type&) override;

	virtual map<string, AbstractScope*>& getNamedScopes()override;
	virtual map<string, Type*>& getTypes() override;
	virtual map<string, FunctionSymbol*>& getFunctions() override;
	virtual map<string, VariableSymbol*>& getVariables() override;
	virtual map<string, StructureSymbol*>& getStructures() override;



	virtual void declareFunction(FunctionSymbol *function) = 0;
	virtual void declareVariable(VariableSymbol *variable) = 0;
	virtual void declareType(Type *type) = 0;
	virtual void declareStructure(StructureSymbol *structure) = 0;
	virtual void declareNamedScope(AbstractScope *scope) = 0;

	virtual string getName() override;
	virtual void setName(string str) override;

	virtual void setParentScope(AbstractScope *parentScope) override;
	virtual AbstractScope* getParentScope() override;

	virtual void dump(ostream *out, string shift = "") = 0;

	BasicScope();
	BasicScope(AbstractScope *, string name = "");

	bool isDefined(string name);
};

#endif