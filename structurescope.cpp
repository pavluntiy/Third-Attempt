#include "structurescope.hpp"

void StructureScope::dump(ostream *out, string shift){
	*out << shift << "Scope of structure '" << this->getName() << "':\n";

	dumpTypes(out, shift);

	dumpFunctions(out, shift);

	dumpVariables(out, shift);

	dumpStructures(out, shift);


	*out << shift <<  "end of struct scope '" << this->getName() << "';\n" << shift << "=====\n";

}


StructureScope::StructureScope(){}

StructureScope::StructureScope(AbstractScope *scope, string name):BasicScope(scope, name)
{

}

string StructureScope::toString(string shift){
	stringstream result;
	result << shift << "Scope of structure '" << this->getName() << "':\n";

	result << typesToString(shift);

	result << functionsToString(shift);

	result << variablesToString(shift);

	result << structuresToString(shift);

	result << shift <<  "end of struct scope '" << this->getName() << "';\n" << shift << "=====\n";
	return result.str();
}


