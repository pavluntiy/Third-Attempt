#include "functionscope.hpp"


// void FunctionScope::declareFunction(FunctionSymbol function){
// 	throw NoticeException("No nested functions are allowed!");
// }

void FunctionScope::declareFunction(FunctionSymbol *function){
	string name = function->getName();

	if(this->variables.count(name) || this->functions.count(name)){
		throw NoticeException("Function '" + name + "' redeclaration!");
	}

	this->functions[name] = function;
}

void FunctionScope::declareVariable(VariableSymbol *variable){
	string name = variable->getName();

	if(this->variables.count(name) || this->functions.count(name)){
		throw NoticeException("Variable '" + name + "' redeclaration!");
	}

	this->variables[name] = variable;
}

void FunctionScope::declareType(Type *type){
	throw NoticeException("No nested types are allowed!");
}

void FunctionScope::declareStructure(StructureSymbol *structure){
	throw NoticeException("No nested structures are allowed!");
}

void FunctionScope::dump(ostream *out, string shift){
	*out << shift << "Scope of function '" << this->getName() << "':\n";

	*out << shift << "\tTypes:\n";
	for(auto it: this->types){
		*out << shift << "\t\t" << it.first << "\n";
	}

	*out << shift << "\tFunctions:\n";
	for(auto it: this->functions){
		*out << shift << "\t\t" << it.first << "\n";
		it.second->getFunctionScope()->dump(out, shift + "\t\t");
	}

	*out << shift << "\tVariables:\n";
	for(auto it: this->variables){
		*out << shift << "\t\t" << it.first << "\n";
	}

	*out << shift <<  "end of scope '" << this->getName() << "';\n=====\n";

}


FunctionScope::FunctionScope(){}

FunctionScope::FunctionScope(AbstractScope *scope, string name):BasicScope(scope, name)
{

}

void FunctionScope::setReturnType(Type *type){
	this->returnType = type;
}



