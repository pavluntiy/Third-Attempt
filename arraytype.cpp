#include "arraytype.hpp"


// bool Type::modifiersEqual(const Type &other) 
// const{

// 	auto otherStorageModes = other.getStorageModes();
// 	if(this->storageModes.size() != otherStorageModes.size()){
// 		return false;
// 	}


// 	for(int i = 0; i < this->storageModes.size(); ++i){
// 		if(this->storageModes[i] != otherStorageModes[i]){
// 			return false;
// 		}
// 	}

// 	auto otherModifiers = other.getModifiers();
// 	if(this->modifiers.size() != otherModifiers.size()){
// 		return false;
// 	}

// 	for(int i = 0; i < this->modifiers.size(); ++i){
// 		if(this->modifiers[i] != otherModifiers[i]){
// 			return false;
// 		}
// 	}

// 	auto otherAccessModes = other.getAccessModes();
// 	if(this->accessModes.size() != otherAccessModes.size()){
// 		return false;
// 	}

// 	for(int i = 0; i < this->accessModes.size(); ++i){
// 		if(this->accessModes[i] != otherAccessModes[i]){
// 			return false;
// 		}
// 	}

// 	auto otherDimensions = other.getDimensions();
// 	if(this->dimensions.size() != otherDimensions.size()){
// 		return false;
// 	}

// 	for(int i = 0; i < this->dimensions.size(); ++i){
// 		if(this->dimensions[i] != otherDimensions[i]){
// 			return false;
// 		}
// 	}

// 	return true;
// }

bool ArrayType::operator==(const Type &other)
const{

	return this->name == other.getName() && this->modifiersEqual(other);

}

const vector<int>& ArrayType::getDimensions()const{
	return this->dimensions;
}

bool ArrayType::operator==(Type *other)
const{
	return *this == *other;
}

string ArrayType::toString(string shift){
	stringstream result;
	result << shift;
	result << "Array ";

	for(auto it: this->getDimensions()){
		result <<" [" << it + "] ";
	}

	result << "of " << this->basicType->toString();

	result << " '" << this->getName() << "'";
	result << " at " << this;
	return result.str();
}


bool ArrayType::isArray(){
	return true;
}

