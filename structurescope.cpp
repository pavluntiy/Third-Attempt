#include "structurescope.hpp"

void StructureScope::declareFunction(FunctionSymbol *function){
	string name = function->getName();

	if(this->isFunction(name)){
		FunctionSymbol *tmp = dynamic_cast<FunctionSymbol*>(this->resolve(name));
		if(!tmp->isOnlyDeclared()){
			throw NoticeException("Function '" + name + "' redeclaration, previously defined at " + tmp->getPosition().toString());
		}
		else {
			//do some staff...
			return;
		}
	}
	if(this->isDefined(name)){
		throw NoticeException("Trying to redeclare '" + name + "' as function!");
	}


	this->functions[name] = function;
}

void StructureScope::declareVariable(VariableSymbol *variable){
	string name = variable->getName();

	if(this->isDefined(name)){
		throw NoticeException("Variable '" + name + "' redeclaration!");
	}

	this->variables[name] = variable;
}

void StructureScope::declareType(Type *type){
	string name = type->getName();

	if(this->isDefined(name)){
		throw NoticeException("Type '" + name + "' redeclaration!");
	}

	this->types[name] = type;
}

void StructureScope::declareStructure(StructureSymbol *structure){
	string name = structure->getName();

	if(this->isStructure(name)){
		StructureSymbol *tmp = dynamic_cast<StructureSymbol*>(this->resolve(name));
		if(!tmp->isOnlyDeclared()){
			throw NoticeException("Structure '" + name + "' redeclaration, previously defined at " + tmp->getPosition().toString());
		}
		else {
			//do some staff...
			return;
		}
	}
	if(this->isDefined(name)){
		throw NoticeException("Trying to redeclare '" + name + "' as structure!");
	}


	this->structures[name] = structure;
	this->namedScopes[name] = structure->getStructureScope();	
}

void StructureScope::declareNamedScope(AbstractScope *scope){
	string name = scope->getName();

	if(this->isDefined(name)){
		throw NoticeException("Named scope '" + name + "' redeclaration!");
	}

	this->namedScopes[name] = scope;
}

void StructureScope::dump(ostream *out, string shift){
	*out << shift << "Scope of structure '" << this->getName() << "':\n";

	*out << shift << "\tTypes:\n";
	for(auto it: this->types){
		*out << shift << "\t\t" << it.second->toString() << "\n";
	}

	*out << shift <<"\tFunctions:\n";
	for(auto it: this->functions){
		*out << shift << "\t\t" << it.second->toString() << "\n";
		*out << shift << "\t\t" << it.second->argumentsToString() << "\n";
		it.second->getFunctionScope()->dump(out, shift + "\t\t");
	}

	*out << shift <<"\tVariables:\n";
	for(auto it: this->variables){
		*out << shift << "\t\t" << it.second->toString() << "\n";
	}

	*out << shift <<"\tStructures:\n";
	for(auto it: this->structures){
		*out << shift << "\t\t" << it.second->toString() << "\n";
		it.second->getStructureScope()->dump(out, shift + "\t\t");
	}


	*out << shift <<  "end of struct scope '" << this->getName() << "';\n" << shift << "=====\n";

}


StructureScope::StructureScope(){}

StructureScope::StructureScope(AbstractScope *scope, string name):BasicScope(scope, name)
{

}


