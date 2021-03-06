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

	dumpVariables(out, shift);

	*out << shift << "end of local scope;\n" << shift << "=====\n";

}

string LocalScope::toString(string shift){
	stringstream result;
	result<< shift << "Local scope'" << this->getName() << "':\n";

	result << variablesToString(shift);

	result << shift <<  "end of local scope;\n" << shift << "=====\n";

	return result.str();
}

