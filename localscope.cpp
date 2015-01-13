#include "localscope.hpp"

void LocalScope::declareFunction(FunctionSymbol *function){
	throw NoticeException("No local functions are allowed");
}

void LocalScope::declareType(Type *type){
	throw NoticeException("No local types are allowed");
}

void LocalScope::declareStructure(StructureSymbol *structure){
	throw NoticeException("No local sturctures are allowed");
}

void LocalScope::declareNamedScope(AbstractScope *scope){
	throw NoticeException("No local namedScopes are allowed");
}


LocalScope::LocalScope(AbstractScope *parentScope, string name):BasicScope(parentScope, name)
{

}

void LocalScope::dump(ostream *out, string shift){
	*out << shift << "Local scope " << this->getName() << ":\n";

	*out << shift << "\tVariables:\n";
	for(auto it: this->variables){
		*out << shift << "\t\t" << it.second->toString() << "\n";
	}

	*out << shift << "end of local scope;\n" << shift << "=====\n";

}

