#include "importsymbol.hpp"

BasicScope* ImportSymbol::getScope(){
	return this->scope;
}

void ImportSymbol::setScope(BasicScope *scope){
	this->scope = scope;
}



CompoundNameNode* ImportSymbol::getModuleName(){
	return this->moduleName;
}

CompoundNameNode* ImportSymbol::getScopeName(){
	return this->scopeName;
}

void ImportSymbol::setModuleName(CompoundNameNode *moduleName){
	this->moduleName = moduleName;
}

void ImportSymbol::setScopeName(CompoundNameNode *scopeName){
	this->scopeName = scopeName;
}

string ImportSymbol::toString(string shift){
	stringstream result;
	// result << shift <<  "'" << this->getName() << "' at ";
	// result << this;
	// result << " of " << this->getType()->toString();
	// result << " (defined at " << this->getPosition().toString() << ")";
	return result.str();
}