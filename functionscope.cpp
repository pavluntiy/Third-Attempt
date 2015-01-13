#include "functionscope.hpp"

void FunctionScope::declareType(Type *type){
	throw NoticeException("No nested types are allowed!");
}

void FunctionScope::declareStructure(StructureSymbol *structure){
	throw NoticeException("No nested structures are allowed!");
}

void FunctionScope::dump(ostream *out, string shift){
	*out << shift << "Scope of function '" << this->getName() << "':\n";


	*out << shift << "\tFunctions:\n";
	for(auto it: this->functions){
		*out << shift <<"\t\t" << it.first << "\n";
		*out << shift << "\t\t" << it.second->toString() << "\n";
		it.second->getFunctionScope()->dump(out, shift + "\t\t");
	}

	*out << shift << "\tVariables:\n";
	for(auto it: this->variables){
		*out << shift <<"\t\t" << it.second->toString() << "\n";
	}

	*out << shift <<  "end of function scope '" << this->getName() << "';\n" << shift << "=====\n";
}


FunctionScope::FunctionScope(){}

FunctionScope::FunctionScope(AbstractScope *scope, string name):BasicScope(scope, name)
{

}

void FunctionScope::setReturnType(Type *type){
	this->returnType = type;
}



