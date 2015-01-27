#ifndef STRUCTURESCOPE
#define STRUCTURESCOPE

#include "header.hpp"
#include "basicscope.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"

class StructureScope: public BasicScope {

string name;
Type *structureType;
public:

	void setStructureType(Type *type);
	//virtual void declareFunction(FunctionSymbol *function) override;
	//virtual void declareVariable(VariableSymbol *variable) override;
	//virtual void declareType(Type *type) override;
	//virtual void declareStructure(StructureSymbol *structure) override;
	//virtual void declareNamedScope(AbstractScope *scope) override;

	StructureScope();
	StructureScope(AbstractScope *scope, string name = "");
	virtual void dump(ostream *out, string shift = "") override;
	virtual string toString(string shift = "") override;


};

#endif