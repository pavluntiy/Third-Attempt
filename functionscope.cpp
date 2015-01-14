#include "functionscope.hpp"

void FunctionScope::declareType(Type *type){
	throw NoticeException("No nested types are allowed!");
}

void FunctionScope::declareStructure(StructureSymbol *structure){
	throw NoticeException("No nested structures are allowed!");
}

void FunctionScope::dump(ostream *out, string shift){
	*out << shift << "Scope of function '" << this->getName() << "':\n";

	dumpFunctions(out, shift);

	dumpVariables(out, shift);

	*out << shift <<  "end of function scope '" << this->getName() << "';\n" << shift << "=====\n";
}


FunctionScope::FunctionScope(){}

FunctionScope::FunctionScope(AbstractScope *scope, string name):BasicScope(scope, name)
{

}

void FunctionScope::setReturnType(Type *type){
	this->returnType = type;
}



