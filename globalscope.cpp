#include "globalscope.hpp"

void GlobalScope::declareFunction(FunctionSymbol function){
	throw NoticeException("No nested functions are allowed!");
}

void GlobalScope::declareVariable(VariableSymbol variable){
	string name = variable.getName();

	if(this->variables.count(name) || this->functions.count(name)){
		throw NoticeException("Variable '" + name + "' redeclaration!");
	}

	this->variables[name] = variable;
}

void GlobalScope::declareType(Type type){
	this->types[type.getName()] = type;
}

FunctionSymbol* GlobalScope::resolveFunction(string name){
	if(this->functions.count(name)){
		return &functions[name];
	}
	throw NoticeException("Undeclared function '"+ name + "'!");
}

VariableSymbol* GlobalScope::resolveVariable(string name){
	if(this->variables.count(name)){
		return &variables[name];
	}
	throw NoticeException("Undeclared variable '"+ name + "'!");
}

Type* GlobalScope::resolveType(string name){
	if(this->types.count(name)){
		return &types[name];
	}
	throw NoticeException("Undeclared type '"+ name + "'!");
}

Type* GlobalScope::resolveType(Type type){
	string name = type.getName();
	if(this->types.count(name)){
		//if(all specs bla-bla-bla)
		return &types[name];
	}
	throw NoticeException("Undeclared type '"+ name + "'!");
}

GlobalScope::GlobalScope(){
	this->name = "global";
	this->declareType(Type("int", 8));
	this->declareType(Type("char", 1));
}

void GlobalScope::dump(ostream *out){
	*out << " Global Scope:\n";

	*out << "\tTypes:\n";
	for(auto it: this->types){
		*out << "\t\t" << it.first << "\n";
	}

	*out << "\tFunctions:\n";
	for(auto it: this->functions){
		*out << "\t\t" << it.first << "\n";
	}

	*out << "\tVariables:\n";
	for(auto it: this->variables){
		*out << "\t\t" << it.first << "\n";
	}

	*out << "end of global scope ';\n=====\n";

}

