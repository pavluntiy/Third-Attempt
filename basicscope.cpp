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

map<Type*, StructureSymbol*>& BasicScope::getTypeStructures(){
	return this->typeStructures;
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

StructureSymbol* BasicScope::resolveStructure(Type *type){
		
	string name = type->getName();
	auto currentScope = this->resolveNamedScope(type->getFullName());
	while(currentScope != nullptr){
		if(currentScope->getTypes().count(name)){
			break;
		}
		currentScope = currentScope->getParentScope();
	}

	if(!currentScope || !currentScope->getTypes().count(name)){
		throw NoticeException("Undeclared type'"+ name + "'!");	
	}

	auto typeStructures = currentScope->getTypeStructures();

	if(!typeStructures.count(type)){
		throw TypeException("No way to resolve type " + type->toString() + " as structure!");
	}

	return typeStructures[type];
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

pair<bool, vector<FunctionSymbol*>> BasicScope::getFunctionCallConversions(FunctionCallSymbol *functionCall, FunctionType *candidate){
	auto candidateArgTypes = candidate->getArgumentTypes();
	auto callArgTypes = functionCall->getArgumentTypes();

	vector<FunctionSymbol*> convertingFunctions;

	if(callArgTypes.size() != candidateArgTypes.size() && !candidate->isVarargs()){
		return make_pair(false, convertingFunctions);
	}

	int bound = min(callArgTypes.size(), candidateArgTypes.size());

	for(int i = 0; i < bound; ++i){
		if(callArgTypes[i] == candidateArgTypes[i]){
			convertingFunctions.push_back(nullptr);
		}
		else {
			auto tmp = candidateArgTypes[i]->getConversion(callArgTypes[i]);
			if(!tmp.first){
				return make_pair(false, convertingFunctions);
			}
			else{
				convertingFunctions.push_back(tmp.second);
			}
		}
	}

	return make_pair(true, convertingFunctions);
}

bool BasicScope::checkFunctionType(FunctionCallSymbol *functionCall, FunctionType *candidate){
	auto candidateArgTypes = candidate->getArgumentTypes();
	auto callArgTypes = functionCall->getArgumentTypes();
	if(callArgTypes.size() != candidateArgTypes.size()){
		return false;
	}

	for(int i = 0; i < candidateArgTypes.size(); ++i){
		if(callArgTypes[i] != candidateArgTypes[i]){
			return false;
		}
	}

	return true;
}

pair<FunctionSymbol*, vector<FunctionSymbol*>> BasicScope::resolveFunctionCall(FunctionCallSymbol* functionCall, CompoundNameNode* functionName){
	try{
			//cout << "!" << functionCall->getFullName()->getSimpleName() << "!\n";
			//cout << functionCall->toString() << "\n";
		auto functionList = getOverloadedFunctionList(functionName);

		for(auto it: functionList){
			if(checkFunctionType(functionCall, it->getFunctionType())){
				return make_pair(it, vector<FunctionSymbol*>());
			}
		}

		for(auto it: functionList){
			auto tmp = this->getFunctionCallConversions(functionCall, it->getFunctionType());
			if(tmp.first){
				return make_pair(it, tmp.second);
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
	//cout << "!!!:::" << name << endl;
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



Type* BasicScope::resolveModifiedType(Type *type){
	//cout << "ololo" << endl;
	//cout << "resolving type " << (new Type(type))->toString() << "\n";
	string name = type->getName();
	AbstractScope *currentScope = this->resolveNamedScope(type->getFullName());



	while(currentScope != nullptr){
		//cout << name << "!\n";
		//currentScope->dump(&cout);
		if(currentScope->getTypes().count(name)){
			break;
		}
		currentScope = currentScope->getParentScope();
	}

	if(!currentScope || !currentScope->getTypes().count(name)){
		throw NoticeException("Undeclared type'"+ name + "'!");	
	}

	auto typeFamily = currentScope->getTypes()[name];

//	cout << "Typefamily: " << name << ":\n";
	for(auto it: typeFamily){
//		cout << "Trying... " << it->toString() << "\n";

//	cout << "Comparing " << (new Type(type))->toString() << " && " << it->toString() << "...\n";
		if(type->modifiersEqual(*it)){
			return it;
		}
	}

//	cout << "...Fail!...\b";


	//auto result = new Type(type);
	//cout << "Now new type " << result->toString() << "\n";
	currentScope->addType(type->getName(), type);
	return type;
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
	AbstractScope *previousScope = this;
	while(currentScope != nullptr){
		if(currentScope->isDefined(simpleName)){
			return currentScope->resolve(simpleName);
		}
		previousScope = currentScope;
		currentScope = currentScope->getParentScope();
	}
	throw NoticeException("Undeclared symbol '"+ simpleName + "'!\n" + previousScope->toString());
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
	//cout << "Pushed to " << name << " type " << type->toString() << "\n";
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

	//cout << "!!" << name << "\n";

	if(this->isDefined(name)){
		throw NoticeException("Type '" + name + "' redeclaration #1!");
	}

	addType(name, type);
}

void BasicScope::declareType(string name, Type *type){

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
	addTypeStructure(structure->getStructureType(), structure);
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

string BasicScope::functionsToString(string shift){
	stringstream ss;
	ss << shift << "\tFunctions:\n";
	for(auto it: this->functions){
		for(auto it2: it.second){
			ss << shift << "\t\t" << it2->toString() << "\n";
			ss << shift << "\t\t" << it2->argumentsToString() << "\n";
			it2->getFunctionScope()->toString(shift + "\t\t");
		}
	}	

	return ss.str();
}

string BasicScope::variablesToString(string shift){
	stringstream ss;
	ss << shift << "\tVariables:\n";
	for(auto it: this->variables){
		ss << "\t\t" << shift << it.second->toString() << "\n";
	}
	return ss.str();
}

string BasicScope::structuresToString(string shift){
	stringstream ss;
	ss << shift << "\tStructures:\n";
	for(auto it: this->structures){
		ss << "\t\t" << shift << it.second->toString(shift + "\t\t") << "\n";
	}
	return ss.str();
}
string BasicScope::typesToString(string shift){
	stringstream ss;
	ss << shift << "\tTypes:\n";
	for(auto it: this->types){
		for(auto it2: it.second){
			ss << shift << "\t\t" << it2->toString() << "\n";
		}
	}

	return ss.str();
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

map<string, BasicNode*>& BasicScope::getModules(){
	return this->modules;
}


void BasicScope::addModule(BasicNode *tree, string name){
	// if(this->modules.count(name)){
	// 	//throw TypeException("Reimporting module " + name);
	// 	return;
	// }
	this->modules[name] = tree;
	//cout << "Added  " + name << "\n";
}

void BasicScope::addTypeStructure(Type *type, StructureSymbol *structure){
	this->typeStructures[type] = structure;
}

BasicNode* BasicScope::resolveModule(string name){
	if(!this->modules.count(name)){
		return nullptr;
	}
	return modules[name];
}

bool BasicScope::hasModule(string name){
	if(!this->modules.count(name)){
		return false;
	}
	return true;
}
