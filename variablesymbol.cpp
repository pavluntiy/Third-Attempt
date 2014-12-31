#include "variablesymbol.hpp"

void VariableSymbol::setBase(string str){
	this->base = str;
}

void VariableSymbol::setOffset(int offset){
	this->offset = offset;
}

string VariableSymbol::getAddress(){
	return this->base + " + " + to_string(this->offset);
}

string VariableSymbol::getBase(){
	return this->base;
}

int VariableSymbol::getOfsset(){
	return this->offset;
}

int VariableSymbol::getSize(){
	return this->type->getSize();
}