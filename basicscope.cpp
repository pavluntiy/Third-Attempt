#include "basicscope.hpp"


// BasicScope::BasicScope(){}
// BasicScope::BasicScope(AbstractScope*){}

string BasicScope::getName(){
	return this->name;
}

void BasicScope::setName(string str){
	this->name = str;
}

void BasicScope::setParentScope(AbstractScope *parentScope){
	this->parentScope = parentScope;
}

AbstractScope* BasicScope::getParentScope(){
	return this->parentScope;
}

map<string, Type*>& BasicScope::getTypes(){
	return this->types;
}

map<string, FunctionSymbol*>& BasicScope::getFunctions(){
	return this->functions;
}

map<string, VariableSymbol*>& BasicScope::getVariables(){
	return this->variables;
}

map<string, AbstractScope*>& BasicScope::getNamedScopes(){
	return this->namedScopes;
}

map<string, StructureSymbol*>& BasicScope::getStructures(){
	return this->structures;
}

FunctionSymbol* BasicScope::resolveFunction(CompoundNameNode *name){
	AbstractScope *currentScope = resolveNamedScope(name);
	string simpleName = name->getSimpleName();
	while(currentScope != nullptr){
		if(currentScope->getFunctions().count(simpleName)){
			return currentScope->getFunctions()[simpleName];
		}
		currentScope = currentScope->getParentScope();
	}
	throw NoticeException("Undeclared function '"+ simpleName + "'!");
}

VariableSymbol* BasicScope::resolveVariable(CompoundNameNode *name){
	AbstractScope *currentScope = resolveNamedScope(name);
	string simpleName = name->getSimpleName();
	while(currentScope != nullptr){
		if(currentScope->getVariables().count(simpleName)){
			return currentScope->getVariables()[simpleName];
		}
		currentScope = currentScope->getParentScope();
	}
	throw NoticeException("Undeclared variable '"+ simpleName + "'!");
}

Type* BasicScope::resolveType(CompoundNameNode *name){
	AbstractScope *currentScope = resolveNamedScope(name);
	string simpleName = name->getSimpleName();
	while(currentScope != nullptr){
		if(currentScope->getTypes().count(simpleName)){
			return currentScope->getTypes()[simpleName];
		}
		currentScope = currentScope->getParentScope();
	}
	throw NoticeException("Undeclared type '"+ simpleName + "'!");
}

Type* BasicScope::resolveType(Type *type){
	string name = type->getName();
	AbstractScope *currentScope = this->resolveNamedScope(type->getFullName());
	while(currentScope != nullptr){
		if(currentScope->getTypes().count(name)){
			return currentScope->getTypes()[name];
		}
		currentScope = currentScope->getParentScope();
	}
	
	throw NoticeException("Undeclared type'"+ name + "'!");
}

Type* BasicScope::resolveType(const Type &type){
	string name = type.getName();
	AbstractScope *currentScope = this->resolveNamedScope(type.getFullName());
	while(currentScope != nullptr){
		if(currentScope->getTypes().count(name)){
			return currentScope->getTypes()[name];
		}
		currentScope = currentScope->getParentScope();
	}
	
	throw NoticeException("Undeclared type'"+ name + "'!");
}

AbstractScope* BasicScope::resolveNamedScope(const vector<string>& longName){

	AbstractScope *currentScope = this;

	if(longName.size() == 1){
		return currentScope;
	}

	string foundNames = "";

	while(currentScope != nullptr){
		if(currentScope->getNamedScopes().count(longName[0])){
			for(int i = 0; i < static_cast<int>(longName.size()) - 1; ++i){
				if(!currentScope->getNamedScopes().count(longName[i])){
					throw NoticeException("No " + longName[i] + " found in " +foundNames);
				}
				currentScope = currentScope->getNamedScopes()[longName[i]];
				foundNames += "." + longName[i];
			}
			return currentScope;
		}
		currentScope = currentScope->getParentScope();
	}
	
	throw NoticeException("Undeclared name in named scopes'"+ foundNames + "'!");
}

AbstractScope* BasicScope::resolveNamedScope(CompoundNameNode* node){


	AbstractScope *currentScope = this;

	if(node->isSimpleName()){
		return currentScope;
	}

	string name = node->getName();
	string foundNames = "";

	auto nodeNames = node->getNames();
	while(currentScope != nullptr){
		if(currentScope->getNamedScopes().count(nodeNames[0])){
			for(int i = 0; i < static_cast<int>(nodeNames.size()) - 1; ++i){
				if(!currentScope->getNamedScopes().count(nodeNames[i])){
					throw NoticeException("No " + nodeNames[i] + " found in " +foundNames);
				}
				currentScope = currentScope->getNamedScopes()[nodeNames[i]];
				foundNames += "." + nodeNames[i];
			}
			return currentScope;
		}
		currentScope = currentScope->getParentScope();
	}
	
	throw NoticeException("Undeclared name in named scopes'"+ name + "'!");
}

BasicScope::BasicScope(){

}

BasicScope::BasicScope(AbstractScope *parentScope, string name){
	this->parentScope = parentScope;
	this->name = name;
}

bool BasicScope::isDefined(string name){
	return
		this->variables.count(name) 
		|| 
		this->functions.count(name) 
		|| 
		this->namedScopes.count(name) 
		||
		this->structures.count(name) 
	;
}

bool BasicScope::isFunction(string name){
	return this->functions.count(name);
}

bool BasicScope::isVariable(string name){
	return this->variables.count(name);
}

bool BasicScope::isStructure(string name){
	return this->structures.count(name);
}

bool BasicScope::isNamedScope(string name){
	return this->namedScopes.count(name);
}

BasicSymbol* BasicScope::resolve(string name){
	if(isVariable(name)){
		return this->variables[name];
	}

	if(isFunction(name)){
		return this->functions[name];
	}

	if(isStructure(name)){
		return this->structures[name];
	}

	throw TypeException("What are you trying to resolve!?");
}



// void BasicScope::dump(ostream *out, string shift){}
// void BasicScope::declareFunction(FunctionSymbol function){}
// void BasicScope::declareVariable(VariableSymbol variable){}
// void BasicScope::declareType(Type type){}
