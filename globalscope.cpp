#include "globalscope.hpp"

void GlobalScope::declareFunction(FunctionSymbol *function){
	string name = function->getName();

	if(this->isDefined(name)){
		throw NoticeException("Function '" + name + "' redeclaration!");
	}

	this->functions[name] = function;
}

void GlobalScope::declareVariable(VariableSymbol *variable){
	string name = variable->getName();

	if(this->isDefined(name)){
		throw NoticeException("Variable '" + name + "' redeclaration!");
	}

	this->variables[name] = variable;
}

void GlobalScope::declareType(Type *type){
	string name = type->getName();

	if(this->isDefined(name)){
		throw NoticeException("Type '" + name + "' redeclaration!");
	}

	this->types[name] = type;
}

void GlobalScope::declareStructure(StructureSymbol *structure){
	string name = structure->getName();
	if(this->isDefined(name)){
		throw NoticeException("Structure '" + name + "' redeclaration!");
	}
	this->structures[name] = structure;
	this->namedScopes[name] = structure->getStructureScope();
}

void GlobalScope::declareNamedScope(AbstractScope *scope){
	string name = scope->getName();

	if(this->isDefined(name)){
		throw NoticeException("Named scope '" + name + "' redeclaration!");
	}

	this->namedScopes[name] = scope;
}


GlobalScope::GlobalScope():BasicScope(nullptr, "global")
{
	this->declareType(new Type("int", 8));
	this->declareType(new Type("char", 1));
	this->declareNamedScope(this);
}

void GlobalScope::dump(ostream *out, string shift){
	*out << shift << "Global Scope:\n";

	*out << shift << "\tTypes:\n";
	for(auto it: this->types){
		*out << shift << "\t\t" << it.second->toString() << "\n";
	}

	*out << "\tFunctions:\n";
	for(auto it: this->functions){
		*out << "\t\t" << it.second->toString() << "\n";
		*out << "\t\t" << it.second->argumentsToString() << "\n";
		it.second->getFunctionScope()->dump(out, shift + "\t\t");
	}

	*out << "\tVariables:\n";
	for(auto it: this->variables){
		*out << "\t\t" << it.second->toString() << "\n";
	}

	*out << "\tStructures:\n";
	for(auto it: this->structures){
		*out << "\t\t" << it.first << "\n";
		it.second->getStructureScope()->dump(out, shift + "\t\t");
	}

	*out << "end of global scope;\n" << shift << "=====\n";

}

