#include "localscope.hpp"

void LocalScope::declareFunction(FunctionSymbol *function){
	throw NoticeException("No local functions are allowed");
	// string name = function->getName();

	// if(this->isFunction(name)){
	// 	FunctionSymbol *tmp = dynamic_cast<FunctionSymbol*>(this->resolve(name));
	// 	if(!tmp->isOnlyDeclared()){
	// 		throw NoticeException("Function '" + name + "' redeclaration, previously defined at " + tmp->getPosition().toString());
	// 	}
	// 	else {
	// 		//do some staff...
	// 		return;
	// 	}
	// }

	// if(this->isDefined(name)){
	// 	throw NoticeException("Trying to redeclare '" + name + "' as function!");
	// }


	// this->functions[name] = function;
}

void LocalScope::declareVariable(VariableSymbol *variable){
	string name = variable->getName();

	if(this->isDefined(name)){
		throw NoticeException("Variable '" + name + "' redeclaration!");
	}

	this->variables[name] = variable;
}

void LocalScope::declareType(Type *type){
	throw NoticeException("No local types are allowed");
	// string name = type->getName();

	// if(this->isDefined(name)){
	// 	throw NoticeException("Type '" + name + "' redeclaration!");
	// }

	// this->types[name] = type;
}

void LocalScope::declareStructure(StructureSymbol *structure){
	throw NoticeException("No local sturctures are allowed");
	// string name = structure->getName();
	// if(this->isStructure(name)){
	// 	StructureSymbol *tmp = dynamic_cast<StructureSymbol*>(this->resolve(name));
	// 	if(!tmp->isOnlyDeclared()){
	// 		throw NoticeException("Structure '" + name + "' redeclaration, previously defined at " + tmp->getPosition().toString());
	// 	}
	// 	else {
	// 		//do some staff...
	// 		return;
	// 	}
	// }
	// if(this->isDefined(name)){
	// 	throw NoticeException("Trying to redeclare '" + name + "' as structure!");
	// }

	// this->structures[name] = structure;
	// this->namedScopes[name] = structure->getStructureScope();
	// this->types[name] = structure->getStructureType();
}

void LocalScope::declareNamedScope(AbstractScope *scope){
	throw NoticeException("No local namedScopes are allowed");
	// string name = scope->getName();

	// if(this->isDefined(name)){
	// 	throw NoticeException("Named scope '" + name + "' redeclaration!");
	// }

	// this->namedScopes[name] = scope;
}


LocalScope::LocalScope(AbstractScope *parentScope):BasicScope(parentScope)
{

}

void LocalScope::dump(ostream *out, string shift){
	*out << shift << "Local scope:\n";


	// *out << "\tFunctions:\n";
	// for(auto it: this->functions){
	// 	*out << "\t\t" << it.second->toString() << "\n";
	// 	*out << "\t\t" << it.second->argumentsToString() << "\n";
	// 	it.second->getFunctionScope()->dump(out, shift + "\t\t");
	// }

	*out << "\tVariables:\n";
	for(auto it: this->variables){
		*out << "\t\t" << it.second->toString() << "\n";
	}

	// *out << "\tStructures:\n";
	// for(auto it: this->structures){
	// 	*out << "\t\t" << it.second->toString()<< "\n";
	// 	it.second->getStructureScope()->dump(out, shift + "\t\t");
	// }

	*out << "end of local scope;\n" << shift << "=====\n";

}

