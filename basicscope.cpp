#include "basicscope.hpp"


// string BasicScope::getName(){
// 	return this->name;
// }

// void BasicScope::setName(string str){
// 	this->name = str;
// }

void BasicScope::setParentScope(BasicScope *parentScope){
	this->parentScope = parentScope;
}

BasicScope* BasicScope::getParentScope(){
	return this->parentScope;
}

