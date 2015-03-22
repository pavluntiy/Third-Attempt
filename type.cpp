#include "type.hpp"

// string Type::getName() const{
// 	return this->name;
// }

int Type::getSize() const{
	return this->size;
}

Type::Type(string name, int size){
	this->name = name;
	this->size = size;
	this->fullName.push_back(name);
}
Type::Type(){}

Type::Type(TypeNode *node){
	this->name = node->getName()->getSimpleName();
	this->storageModes = node->getStorageModes();
	this->modifiers = node->getModifiers();
	this->accessModes = node->getAccessModes();
	this->fullName = node->getName()->getNames();
	for(auto it: node->getDimensions()){
		this->dimensions.push_back(stoi(it->getText()));
	}

	if(
		this->storageModes.size() || this->modifiers.size() 
		|| 
		this->accessModes.size() || this->dimensions.size()
	){
		this->unqualified = false;
	}
	else {
		this->unqualified = true;
	}
}
const vector<string>& Type::getFullName()const {
	return this->fullName;
}

const vector<string>& Type::getStorageModes()const{
	return this->storageModes;
}

const vector<string>& Type::getModifiers()const{
	return this->modifiers;
}

const vector<string>& Type::getAccessModes()const{
	return this->accessModes;
}

const vector<int>& Type::getDimensions()const{
	return this->dimensions;
}

bool Type::modifiersEqual(const Type &other) 
const{

	auto otherStorageModes = other.getStorageModes();
	if(this->storageModes.size() != otherStorageModes.size()){
		return false;
	}


	for(int i = 0; i < this->storageModes.size(); ++i){
		if(this->storageModes[i] != otherStorageModes[i]){
			return false;
		}
	}

	auto otherModifiers = other.getModifiers();
	if(this->modifiers.size() != otherModifiers.size()){
		return false;
	}

	for(int i = 0; i < this->modifiers.size(); ++i){
		if(this->modifiers[i] != otherModifiers[i]){
			return false;
		}
	}

	auto otherAccessModes = other.getAccessModes();
	if(this->accessModes.size() != otherAccessModes.size()){
		return false;
	}

	for(int i = 0; i < this->accessModes.size(); ++i){
		if(this->accessModes[i] != otherAccessModes[i]){
			return false;
		}
	}

	auto otherDimensions = other.getDimensions();
	if(this->dimensions.size() != otherDimensions.size()){
		return false;
	}

	for(int i = 0; i < this->dimensions.size(); ++i){
		if(this->dimensions[i] != otherDimensions[i]){
			return false;
		}
	}

	return true;
}

bool Type::operator==(const Type &other)
const{

	return this->name == other.getName() && this->modifiersEqual(other);

}

bool Type::operator==(Type *other)
const{
	return *this == *other;
}

bool Type::isUnqualified(){
	return this->unqualified;
}

string Type::toString(string shift){
	stringstream result;
	result << shift;
	for(auto it: this->getStorageModes()){
		result << it + " ";
	}
	for(auto it: this->getModifiers()){
		result << it + " ";
	}
	for(auto it: this->getAccessModes()){
		result <<  it + " ";
	}

	for(auto it: this->getDimensions()){
		result << to_string(it) + " ";
	}

	result << " '" << this->getName() << "'";
	result << " at " << this;
	return result.str();
}

// bool operator==(Type *a, Type *b){
// 	return *a == *b;
// }

// void Type::tryToAddConversion(FunctionSymbol *function){

// 	if(function->getReturnType() != this){
// 		return;
// 	}

// 	if(function->getArguments().size() != 1){
// 		return;
// 	}

// 	this->convertTo[function->getArguments()[0]] = function;

// }

void Type::addConversion(Type *type, FunctionSymbol *function){
	//cout << "ADDED "  <<"for " << this << " to " << type << " with " << function << "\n";
	convertTo[type] = function;
}

pair<bool, FunctionSymbol*> Type::getConversion(Type *type){
	//cout << "Getting "  <<"for " << this << " to " << type << "!\n";
	if(this->convertTo.count(type)){
		return make_pair(true, this->convertTo[type]);
	}

	return make_pair(false, nullptr);
}

void Type::setActualType(Type* type){
	this->actualType = type;
}

Type* Type::getActualType(){
	return this->actualType;
}