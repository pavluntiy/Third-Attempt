#include "functionscope.hpp"

void FunctionScope::declareFunction(FunctionSymbol function){
	throw NoticeException("No nested functions are allowed!");
}

void FunctionScope::declareVariable(VariableSymbol variable){
	string name = variable.getName();

	if(this->variables.count(name) || this->functions.count(name)){
		throw NoticeException("Variable '" + name + "' redeclaration!");
	}

	this->variables[name] = variable;
}

void FunctionScope::declareType(Type type){
	throw NoticeException("No nested types are allowed!");
}

FunctionSymbol* FunctionScope::resolveFunction(string name){
	if(this->functions.count(name)){
		return &functions[name];
	}
	throw NoticeException("Undeclared function '"+ name + "'!");
}

VariableSymbol* FunctionScope::resolveVariable(string name){
	if(this->variables.count(name)){
		return &variables[name];
	}
	throw NoticeException("Undeclared function '"+ name + "'!");
}

Type* FunctionScope::resolveType(string name){
	if(this->types.count(name)){
		return &types[name];
	}
	throw NoticeException("Undeclared function '"+ name + "'!");
}

Type* FunctionScope::resolveType(Type type){
	string name = type.getName();
	if(this->types.count(name)){
		//if(all specs bla-bla-bla)
		return &types[name];
	}
	throw NoticeException("Undeclared function '"+ name + "'!");
}

FunctionScope::FunctionScope(string name){
	this->name = name;
}
FunctionScope::FunctionScope(){}

FunctionScope::FunctionScope(BasicScope *scope){
	this->parentScope = scope;
}

void FunctionScope::setReturnType(Type *type){
	this->returnType = type;
}

void FunctionScope::dump(ostream *out){
//	*out << "Scope of function '" << this->getName() << "':\n";

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

//	*out << "end of scope '" << this->getName() << "';\n=====\n";

}


