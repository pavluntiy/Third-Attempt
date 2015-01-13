#include "structurescope.hpp"

void StructureScope::dump(ostream *out, string shift){
	*out << shift << "Scope of structure '" << this->getName() << "':\n";

	*out << shift << "\tTypes:\n";
	for(auto it: this->types){
		for(auto it2: it.second){
			*out << shift << "\t\t" << it2->toString() << "\n";
		}
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


