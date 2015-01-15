#include "valuesymbol.hpp"


string ValueSymbol::getAddress(){
	return "none";
}

string ValueSymbol::getBase(){
	"none";
}

int ValueSymbol::getSize(){
	return this->type->getSize();
}

ValueSymbol::ValueSymbol(string value){
	this->value = value;
}

	void ValueSymbol::setType(Type *type){
		this->type = type;
	}

	Type* ValueSymbol::getType(){
		return this->type;
	}



string ValueSymbol::toString(string shift){
	stringstream result;
//	result << shift <<  "'" << this->getName() << "' at ";
	result << this;
	result << " of " << this->getType()->toString();
	result << " (defined at " << this->getPosition().toString() << ")";
	return result.str();
}