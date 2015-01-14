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

	dumpTypes(out, shift);

	dumpFunctions(out, shift);

	dumpVariables(out, shift);

	dumpStructures(out, shift);
	

	*out << "end of global scope;\n" << shift << "=====\n";

}

