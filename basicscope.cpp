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

vector<AbstractScope*>& BasicScope::getAnonymousScopes(){
	return this->anonymousScopes;
}

void BasicScope::declareAnonymousScope(AbstractScope *scope){
	this->anonymousScopes.push_back(scope);
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

StructureSymbol* BasicScope::resolveStructure(const string &name){

	AbstractScope *currentScope = this;
	while(currentScope != nullptr){
		if(currentScope->getStructures().count(name)){
			return currentScope->getStructures()[name];
		}
		currentScope = currentScope->getParentScope();
	}
	
	throw NoticeException("Undeclared structure;'"+ name + "'!");
}

Type* BasicScope::resolveModifiedType(const Type &type){
	string name = type.getName();
	AbstractScope *currentScope = this;//->resolveNamedScope(type.getFullName());
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
			//auto tmp
			return resolveModifiedType(type);
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

BasicSymbol* BasicScope::resolve(CompoundNameNode *name){
	AbstractScope *currentScope = resolveNamedScope(name);
	string simpleName = name->getSimpleName();
	while(currentScope != nullptr){
		if(currentScope->isDefined(simpleName)){
			return currentScope->resolve(simpleName);
		}
		currentScope = currentScope->getParentScope();
	}
	throw NoticeException("Undeclared symbol '"+ simpleName + "'!");
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


void BasicScope::addFunction(string name, FunctionSymbol *function){
	this->functions[name] = function;
}


void BasicScope::addVariable(string name, VariableSymbol *variable){
	this->variables[name] = variable;
}

void BasicScope::addType(string name, Type *type){
	this->types[name] = type;
}

void BasicScope::addNamedScope(string name, AbstractScope *scope){
	this->namedScopes[name] = scope;
}

void BasicScope::addStructure(string name, StructureSymbol *structure){
	this->structures[name] = structure;
}




void BasicScope::declareFunction(FunctionSymbol *function){
	string name = function->getName();

	if(this->isFunction(name)){
		FunctionSymbol *tmp = dynamic_cast<FunctionSymbol*>(this->resolve(name));
		if(!tmp->isOnlyDeclared()){
			throw NoticeException("Function '" + name + "' redeclaration, previously defined at " + tmp->getPosition().toString());
		}
		else {
			//do some staff...
			return;
		}
	}

	if(this->isDefined(name)){
		throw NoticeException("Trying to redeclare '" + name + "' as function!");
	}


	addFunction(name, function);
}

void BasicScope::declareVariable(VariableSymbol *variable){
	string name = variable->getName();

	if(this->isDefined(name)){
		throw NoticeException("Variable '" + name + "' redeclaration!");
	}

	addVariable(name, variable);
}

void BasicScope::declareType(Type *type){
	string name = type->getName();

	if(this->isDefined(name)){
		throw NoticeException("Type '" + name + "' redeclaration!");
	}

	addType(name, type);
}

void BasicScope::declareStructure(StructureSymbol *structure){
	string name = structure->getName();
	if(this->isStructure(name)){
		StructureSymbol *tmp = dynamic_cast<StructureSymbol*>(this->resolve(name));
		if(!tmp->isOnlyDeclared()){
			throw NoticeException("Structure '" + name + "' redeclaration, previously defined at " + tmp->getPosition().toString());
		}
		else {
			//do some staff...
			return;
		}
	}
	if(this->isDefined(name)){
		throw NoticeException("Trying to redeclare '" + name + "' as structure!");
	}

	//this->structures[name] = structure;
	addStructure(name, structure);
	//this->namedScopes[name] = structure->getStructureScope();
	addNamedScope(name, structure->getStructureScope());
	addType(name, structure->getStructureType());
	//this->types[name] = structure->getStructureType();
}

void BasicScope::declareNamedScope(AbstractScope *scope){
	string name = scope->getName();

	if(this->isDefined(name)){
		throw NoticeException("Named scope '" + name + "' redeclaration!");
	}

	//this->namedScopes[name] = scope;
	addNamedScope(name, scope);
}
