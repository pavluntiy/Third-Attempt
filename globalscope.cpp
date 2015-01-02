#include "globalscope.hpp"

void GlobalScope::declareFunction(FunctionSymbol *function){
	string name = function->getName();

	if(this->variables.count(name) || this->functions.count(name)){
		throw NoticeException("Function '" + name + "' redeclaration!");
	}

	this->functions[name] = function;
}

void GlobalScope::declareVariable(VariableSymbol *variable){
	string name = variable->getName();

	if(this->variables.count(name) || this->functions.count(name)){
		throw NoticeException("Variable '" + name + "' redeclaration!");
	}

	this->variables[name] = variable;
}

void GlobalScope::declareType(Type *type){
	this->types[type->getName()] = type;
}

void GlobalScope::declareStructure(StructureSymbol *structure){
	//this->namedScopes[]
	this->structures[structure->getName()] = structure;
}

GlobalScope::GlobalScope():BasicScope(nullptr, "global")
{
	this->declareType(new Type("int", 8));
	this->declareType(new Type("char", 1));
}

void GlobalScope::dump(ostream *out, string shift){
	*out << shift << "Global Scope:\n";

	*out << shift << "\tTypes:\n";
	for(auto it: this->types){
		*out << shift << "\t\t" << it.first << "\n";
	}

	*out << "\tFunctions:\n";
	for(auto it: this->functions){
		*out << "\t\t" << it.first << "\n";
		it.second->getFunctionScope()->dump(out, shift + "\t\t");
	}

	*out << "\tVariables:\n";
	for(auto it: this->variables){
		*out << "\t\t" << it.first << "\n";
	}

	*out << "\tStructures:\n";
	for(auto it: this->structures){
		*out << "\t\t" << it.first << "\n";
		it.second->getStructureScope()->dump(out, shift + "\t\t");
	}

	*out << "end of global scope;\n=====\n";

}

