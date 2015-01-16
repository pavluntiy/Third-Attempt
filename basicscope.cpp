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

//map<string, Type*>& BasicScope::getTypes(){
map<string, vector<Type*> >& BasicScope::getTypes(){
	return this->types;
}

//map<string, FunctionSymbol*>& BasicScope::getFunctions(){
map<string, vector<FunctionSymbol*>>& BasicScope::getFunctions(){
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
			return currentScope->getFunctions()[simpleName][0];
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

Type* BasicScope::getUnqualifiedType(string name){
	return this->types[name][0];
}

Type* BasicScope::resolveType(CompoundNameNode *name){
	AbstractScope *currentScope = resolveNamedScope(name);
	string simpleName = name->getSimpleName();
	while(currentScope != nullptr){
		if(currentScope->getTypes().count(simpleName)){
			//return currentScope->getTypes()[simpleName];
			return currentScope->getUnqualifiedType(simpleName);
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
			//return currentScope->getTypes()[name];
			return currentScope->getUnqualifiedType(name);
		}
		currentScope = currentScope->getParentScope();
	}
	
	throw NoticeException("Undeclared type'"+ name + "'!");
}

Type* BasicScope::resolveType(string name){
	AbstractScope *currentScope = this;
	while(currentScope != nullptr){
		if(currentScope->getTypes().count(name)){
			return currentScope->getUnqualifiedType(name);
		}
		currentScope = currentScope->getParentScope();
	}

	throw NoticeException("Undeclared unqualified type!");
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

vector<FunctionSymbol*> BasicScope::getOverloadedFunctionList(CompoundNameNode *name){
	//cout << this << "::";
	AbstractScope *currentScope = resolveNamedScope(name);
	//cout << currentScope << "\n";
	string simpleName = name->getSimpleName();
	//cout << currentScope->getName() << "\n";
	//cout << simpleName << currentScope->getFunctions().count(simpleName) << '\n';
	while(currentScope != nullptr){
		if(currentScope->getFunctions().count(simpleName)){
			return currentScope->getFunctions()[simpleName];
		}
		currentScope = currentScope->getParentScope();
	}
	throw NoticeException("Undeclared overloaded functions '"+ simpleName + "'!");
}

FunctionSymbol* BasicScope::resolveFunctionCall(FunctionCallSymbol* functionCall){
	try{
			//cout << "!" << functionCall->getFullName()->getSimpleName() << "!\n";
		auto functionList = getOverloadedFunctionList(functionCall->getFullName());

		for(auto it: functionList){
			if(functionCall->exactlyEquals(it)){
				return it;
			}
		}

		for(auto it: functionList){
			if(functionCall->conversionExists(it)){
				return it;
			}
		}
	}
	catch(NoticeException& ne){
	}

	

	throw NoticeException("#!No acceptable overload found for function " + functionCall->toString());
}

FunctionSymbol* BasicScope::resolveFunction(FunctionSymbol* function){
	//cout << "asdfasdfassdfsdfsdf";
	if(!this->isFunction(function->getName())){
		return nullptr;
	}
	auto functionList = getOverloadedFunctionList(function->getFullName());

	for(auto it: functionList){
		if(function->exactlyEquals(it)){
			return it;
		}
	}

	return nullptr;
}

bool BasicScope::hasType(Type *type){

	string name = type->getName();
	cout << "!!!:::" << name << endl;
	AbstractScope *currentScope = this->resolveNamedScope(type->getFullName());


	while(currentScope != nullptr){
		if(currentScope->getTypes().count(name)){
			break;
		}
		currentScope = currentScope->getParentScope();
	}

	if(!currentScope || !currentScope->getTypes().count(name)){
		throw NoticeException("Undeclared type'"+ name + "'!");	
	}

	auto typeFamily = currentScope->getTypes()[name];

	for(auto it: typeFamily){
		if(*type == *it){
			return true;
		}
	}

	return false;
}

Type* BasicScope::resolveModifiedType(const Type &type){
	//cout << "ololo" << endl;
	string name = type.getName();
	AbstractScope *currentScope = this->resolveNamedScope(type.getFullName());



	while(currentScope != nullptr){
		if(currentScope->getTypes().count(name)){
			break;
		}
		currentScope = currentScope->getParentScope();
	}

	if(!currentScope || !currentScope->getTypes().count(name)){
		throw NoticeException("Undeclared type'"+ name + "'!");	
	}

	auto typeFamily = currentScope->getTypes()[name];

	for(auto it: typeFamily){
		if(type == *it){
			return it;
		}
	}


	auto result = new Type(type);
	currentScope->addType(result->getName(), result);
	return result;
}

Type* BasicScope::resolveType(const Type &type){
	string name = type.getName();
	AbstractScope *currentScope = this->resolveNamedScope(type.getFullName());
	while(currentScope != nullptr){
		if(currentScope->getTypes().count(name)){
			//auto tmp
			//return resolveModifiedType(type);
			return currentScope->getUnqualifiedType(name);
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

// bool BasicScope::isDefined(CompoundNameNode *name){
// 	return
// 		this->resolveVariable(name) 
// 		|| 
// 		this->resolveType(name) 
// 		|| 
// 		this->resolveNamedScope(name) 
// 		||
// 		this->resolveFunction(name) 
// 	;
// }

bool BasicScope::isFunction(string name){
	//cout << name << "!!";
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
		return this->functions[name][0];
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
	//cout << " added " << function->toString() << endl;
	this->functions[name].push_back(function);
}


void BasicScope::addVariable(string name, VariableSymbol *variable){
	this->variables[name] = variable;
}

void BasicScope::addType(string name, Type *type){

	this->types[name].push_back(type);
}

void BasicScope::addNamedScope(string name, AbstractScope *scope){
	this->namedScopes[name] = scope;
}

void BasicScope::addStructure(string name, StructureSymbol *structure){
	this->structures[name] = structure;
}




void BasicScope::declareFunction(FunctionSymbol *function){
	//cout << "Ololo!";
	string name = function->getName();
	if(this->isFunction(name)){	
		//cout << "###__";
		FunctionSymbol *tmp = dynamic_cast<FunctionSymbol*>(this->resolveFunction(function));

		if(!tmp){
			addFunction(name, function);
		}
		else
		if(!tmp->isOnlyDeclared()){
			throw NoticeException("Function '" + name + "' redeclaration, previously defined at " + tmp->getPosition().toString());
		}
		return;
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
		throw NoticeException("Type '" + name + "' redeclaration #1!");
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

void BasicScope::dumpFunctions(ostream *out, string shift){
	*out << "\tFunctions:\n";
	for(auto it: this->functions){
		for(auto it2: it.second){
			*out << "\t\t" << it2->toString() << "\n";
			*out << "\t\t" << it2->argumentsToString() << "\n";
			it2->getFunctionScope()->dump(out, shift + "\t\t");
		}
	}

}

void BasicScope::dumpVariables(ostream *out, string shift){
	*out << "\tVariables:\n";
	for(auto it: this->variables){
		*out << "\t\t" << it.second->toString() << "\n";
	}
}

void BasicScope::dumpStructures(ostream *out, string shift){
	*out << "\tStructures:\n";
	for(auto it: this->structures){
		*out << "\t\t" << it.second->toString()<< "\n";
		it.second->getStructureScope()->dump(out, shift + "\t\t");
	}
}

void BasicScope::dumpTypes(ostream *out, string shift){
	*out << shift << "\tTypes:\n";
	for(auto it: this->types){
		for(auto it2: it.second){
			*out << shift << "\t\t" << it2->toString() << "\n";
		}
	}
}

void BasicScope::import(AbstractScope *other){
	

	auto otherStructures = other->getStructures();
	for(auto it: otherStructures){
		try{
			this->declareStructure(it.second);
		}
		catch(NoticeException &ne){
			throw TypeException("Error importing scope " + other->getName() + ": structure " + it.first + " redeclaration");
		}
	}

	auto otherVariables = other->getVariables();
	for(auto it: otherVariables){
		try{
			this->declareVariable(it.second);
		}
		catch(NoticeException &ne){
			throw TypeException("Error importing scope " + other->getName() + ": variable " + it.first + " redeclaration");
		}
	}

	auto otherTypes = other->getTypes();
	for(auto it: otherTypes){
		for(auto it2: it.second){
			try{
				if(!hasType(it2)){
					this->addType(it2->getName(), it2);
				}
			}
			catch(NoticeException &ne){
				throw TypeException("Error importing scope " + other->getName() + ": type " + it.first + " redeclaration");
			}
		}
	}

	auto otherFunctions = other->getFunctions();
	for(auto it: otherFunctions){
		for(auto it2: it.second){
			try{
				this->declareFunction(it2);
			}
			catch(NoticeException &ne){
				throw TypeException("Error importing scope " + other->getName() + ": function " + it.first + " redeclaration");
			}
		}
	}




}
