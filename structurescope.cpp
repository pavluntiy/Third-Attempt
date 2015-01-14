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


