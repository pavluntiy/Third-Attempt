#include "globalscope.hpp"




GlobalScope::GlobalScope():BasicScope(nullptr, "global")
{	
	this->declareType(new Type("void", 0));
	this->declareType(new Type("float", 8));
	this->declareType(new Type("int", 8));
	this->declareType(new Type("char", 1));
	this->declareNamedScope(this);
}

void GlobalScope::dump(ostream *out, string shift){
	*out << shift << "Global Scope:\n";

	*out << shift << "\tTypes:\n";
	for(auto it: this->types){
		*out << shift << "\t\t" << it.second->toString() << "\n";
	}

	*out << "\tFunctions:\n";
	for(auto it: this->functions){
		*out << "\t\t" << it.second->toString() << "\n";
		*out << "\t\t" << it.second->argumentsToString() << "\n";
		it.second->getFunctionScope()->dump(out, shift + "\t\t");
	}

	*out << "\tVariables:\n";
	for(auto it: this->variables){
		*out << "\t\t" << it.second->toString() << "\n";
	}

	*out << "\tStructures:\n";
	for(auto it: this->structures){
		*out << "\t\t" << it.second->toString()<< "\n";
		it.second->getStructureScope()->dump(out, shift + "\t\t");
	}

	*out << "end of global scope;\n" << shift << "=====\n";

}

