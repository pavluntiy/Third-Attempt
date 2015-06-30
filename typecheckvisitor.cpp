#include "typecheckvisitor.hpp"

void TypeVisitor::visit(ProgramNode *node){
	setCurrentScope(globalScope);

	//auto moduleName = 
	//globalScope->addModule(import(globalScope, "autoimport"), "autoimport");
	node->setAutoImport(import(currentScope, "autoimport"));
	//node->setAutoImport(globalScope->resolveModule("autoimport"));

	if(node->getAutoImport()){
		node->getAutoImport()->accept(this);
	}
	if(node->getChild()){
		node->getChild()->accept(this);
	}
	restoreCurrentScope();
}

void TypeVisitor::visit(BlockNode *node){
	//cout << "This node's scope = " << node->getScope() << "\n";
	if(!node->getScope()){
		//cout << "Hurrah!" << endl;
		LocalScope *localScope = new LocalScope(currentScope, "blockscope " + node->getPosition().toString());
		currentScope->declareAnonymousScope(localScope);
		setCurrentScope(localScope);
		node->setScope(localScope);
		if(node->getChild()){
			node->getChild()->accept(this);
		}
		restoreCurrentScope();
	}
	else {
		if(node->getChild()){
			node->getChild()->accept(this);
		}
	}	
}

void TypeVisitor::visit(OperatorsNode *node){


	// if(this->globalScopeFound){
	// 	LocalScope *localScope = new LocalScope(currentScope);
	// 	currentScope->declareAnonymousScope(localScope);
	// 	setCurrentScope(localScope);

	// 	for(auto it : node->getChildren()){
	// 		it->accept(this);
	// 	}
	// 	restoreCurrentScope();
	// }
	// else {
	// 	this->globalScopeFound = true;
		for(auto it : node->getChildren()){
			it->accept(this);
		}
	//}
}

void TypeVisitor::visit(CompoundNameNode *node){
	node->setSymbol(currentScope->resolve(node));
}

// FunctionCallSymbol* TypeVisitor::setFunctionName(FunctionCallSymbol*){

// }

// FunctionSymbol* getResolvedFunction(FunctionCallSymbol* functionCall, DotNode *){

// }

void TypeVisitor::visit(FunctionCallNode *node){

	//node->setSymbol(currentScope->resolveFunction( dynamic_cast<CompoundNameNode*>(node->getFunctionName())));//Provisional

	auto callee = node->getFunctionName();
	callee->accept(this);
	auto functionCall = new FunctionCallSymbol();

	for(auto it: node->getFunctionArgs()){
		it->accept(this);
		functionCall->addArgumentType(it->getSymbol()->getType());
	}

/*	if(auto tmp = dynamic_cast<FunctionCallNode*>(node->getFunctionName())){
		functionCall->setFunction(tmp);
		auto functionType = dynamic_cast<FunctionType*>(tmp->getType());
		if( !functionCall->exactlyEquals(functionType) && 
			!functionCall->conversionExists(functionType)
		){
			throw TypeException("Invalid functionSymbol call ", node->getPosition());
		}	
	}
	if(auto dotnode = dynamic_cast<DotNode*>(node->getFunctionName())){
		//functionCall->setFullName(dynamic_cast<CompoundNameNode*>(dotNode->getRight()));
		auto object = dotNode->getLeft();
		auto structure = currentScope->resolveStructure(object->getSymbol()->getType());
		auto functionSymbol = structure->getStructureScope()->resolveFunctionCall(functionCall);
		functionCall->setFunction(functionSymbol);
		functionCall->setType(functionSymbol->getReturnType());
	}
    */
	//else{
		auto functionName  = dynamic_cast<CompoundNameNode*>(node->getFunctionName());
		if(!functionName){
			throw TypeException("Not supported way of calling functionSymbol", node->getPosition());
		}
		functionCall->setFullName(functionName);
		auto tmp = currentScope->resolveFunctionCall(functionCall, functionName);
		if(!tmp.first){
			throw TypeException("No overloaded functionSymbol found for " + functionName->toString(), functionName->getPosition());
		}
		auto convertingFunctions = tmp.second;
		auto functionSymbol = tmp.first;
		functionCall->setFunction(functionSymbol);
		functionCall->setType(functionSymbol->getReturnType());
	//}

	if(convertingFunctions.size() > 0){
		//auto conversions = functionCall->getConversions();
		auto arguments = node->getFunctionArgs();
		int bound = convertingFunctions.size();
		for(int i = 0; i < bound; ++i){
			if(convertingFunctions[i]){
				node->replaceArgument(i, insertConversion(arguments[i], convertingFunctions[i]));
			}
		}
	}

	node->setSymbol(functionCall);
	//cout << node->getSymbol()->getType()->toString() << "!!!!!!!!!!!!!!!\n";
	//cout << functionCall->toString() << "!!!!!\n";


	//node->setType(currentScope->resolveType("int"));
}

void TypeVisitor::visit(DotNode *node){

	node->getLeft()->accept(this);
	auto currentSymbol = node->getLeft()->getSymbol();

	setCurrentScope(currentScope->resolveStructure(currentSymbol->getType())->getStructureScope());
	node->getRight()->accept(this);
	auto resultSymbol = node->getRight()->getSymbol();
	node->setSymbol(resultSymbol);
	restoreCurrentScope();
}

BasicNode* TypeVisitor::import(AbstractScope *to, string name){

	if(currentScope->hasModule(name)){
		//return currentScope->resolveModule(name);
		//cout << "Failed: " <<  name << "\n";
		return nullptr;
	}

	currentScope->addModule(nullptr, name);


	auto in = new ifstream(name + ".cch");

	if(!in->is_open()){
		throw TypeException("Bad module " + name);
	}

	Data *data = new Data(*in);
	Lexer lexer(data);

	Parser parser(lexer);
	try{
		parser.buildTree();
	}
	catch (ParserException &pe){

		throw ParserException( "In module '" + name + "' parse error:\n"  + pe.what() );
	}

	TypeVisitor *typeVisitor = new TypeVisitor(to);
	//typeVisitor->setGlobalScope(to);

	try{
		parser.getTree()->accept(typeVisitor);
	}
	catch (NoticeException &ne){
		//isConstruct << ne.what() << '\n';
	}
	catch (TypeException &te){
		throw TypeException("In module " + name + te.what());
	}

	currentScope->addModule(parser.getTree(), name);

	cout << to << ": " << currentScope->resolveModule(name) << " " << name << "\n";

	return parser.getTree();
}

void TypeVisitor::visit(ImportNode *node){


	auto names = node->getModuleName()->getNames();

	string name = ".";

	for(auto it: names){
		name += "/" + it;
	}

	import(currentScope, name);
	node->setTree(currentScope->resolveModule(name));
	//cout << "lalka: " << node->getTree() << "\n";

	// auto in = new ifstream(name + ".cch");

	// if(!in->is_open()){
	// 	throw TypeException("Bad module", node->getPosition());
	// }


	// Lexer lexer(*in);

	// Parser parser(lexer);
	// try{
	// 	parser.buildTree();
	// }
	// catch (ParserException &pe){

	// 	throw ParserException( "In module '" + name + "' parse error:\n"  + pe.what() );
	// }

	// TypeVisitor *typeVisitor = new TypeVisitor();
	// typeVisitor->setGlobalScope(this->currentScope);

	// try{
	// 	parser.getTree()->accept(typeVisitor);
	// }
	// catch (NoticeException &ne){
	// 	//isConstruct << ne.what() << '\n';
	// }
	// catch (TypeException &te){
	// 	throw TypeException("In module " + name + te.what(), node->getPosition());
	// }

	// node->setTree( parser.getTree());
	//this->currentScope->import(typeVisitor->getGlobalScope());




	// *this->out << shift << "( " << node->toString() << endl;
		
	// this->shift.push_back(' ');

 // 	if(node->getModuleName()){
 // 		node->getModuleName()->accept(this);
 // 	}

 // 	if(node->getScopeName()){
 // 		node->getScopeName()->accept(this);
 // 	}

	// this->shift.pop_back();

	// *this->out   << shift << ")" << endl;
}

void TypeVisitor::visit(TypeNode *node){

	Type *type = nullptr;
	for(auto it: node->getDimensions()){
		it->accept(this);
	}

	if(node->getDimensions().size() > 0){
		cout << "ololo!\n\n\n";
		type = new ArrayType(node);

	}
	else {
		type = new Type(node);
	}

	if(type->isArray()){
		cout << "It's an array!\n";
	}

	type = currentScope->resolveType(type);
	node->setSymbol(type);
}

void TypeVisitor::visit(ValueNode *node){

	auto valueSymbol = new ValueSymbol(node->getText());
	switch(node->getType()){
		case ValueNode::Type::INT: {
			valueSymbol->setType(currentScope->resolveType("int"));
		}	
		break;

		case ValueNode::Type::FLOAT: {
			valueSymbol->setType(currentScope->resolveType("float"));
		}	
		break;

		case ValueNode::Type::CHAR: {
			valueSymbol->setType(currentScope->resolveType("char"));
		}	
		break;

		case ValueNode::Type::STRING: {
			valueSymbol->setType(currentScope->resolveType("string"));
		}	
		break;

		default: throw TypeException("What is it!?", node->getPosition());
	}
		
	node->setSymbol(valueSymbol); 
}


void TypeVisitor::visit(VarDeclarationNode *node){
	
	node->getType()->accept(this);
	auto declarationType = dynamic_cast<Type*>(node->getType()->getSymbol());

	for(auto it: node->getVariables()){
		//it.first->accept(this);
		CompoundNameNode *name = it.first;
		string varName = name->getSimpleName();
		setCurrentScope(currentScope->resolveNamedScope(name));
		//cout << currentScope->getName() << '\n';
		auto variable = new VariableSymbol(declarationType, varName);
		variable->setPosition(node->getPosition());
		currentScope->declareVariable(variable);
		it.first->setSymbol(variable);

		// if(it.second){
		// 	it.second->accept(this);
		// }
		restoreCurrentScope();
	}	

}

void TypeVisitor::visit(SignatureNode *node){

	CompoundNameNode *name = node->getName();
	string functionName = name->getSimpleName();

	FunctionType *functionType = new FunctionType;
	setCurrentScope(currentScope->resolveNamedScope(name));


	node->getType()->accept(this);
	//Type* returnType = currentScope->resolveModifiedType(Type(node->getType()));
	Type *returnType = dynamic_cast<Type*>(node->getType()->getSymbol());
	node->getType()->setSymbol(returnType);
	//node->getType()->setSymbol(functionType->getReturnType());
	functionType->setReturnType(returnType);

	//node->setType(functionType);

	FunctionSymbol *functionSymbol = new FunctionSymbol();
	functionSymbol->setFullName(node->getName());
	//functionSymbol->setReturnType(returnType);
	functionSymbol->setPosition(node->getPosition());

	functionSymbol->setType(functionType);
	functionSymbol->setReturnType(returnType);

	FunctionScope *functionScope = new FunctionScope(currentScope, functionName);

	//functionScope->setReturnType(returnType);
	functionSymbol->setFunctionScope(functionScope);

	functionSymbol->setName(functionName);
	functionScope->setName(functionName);
	node->setFunctionSymbol(functionSymbol);
	node->setSymbol(functionSymbol);

	for(auto it: node->getArguments()){
		get<0>(it)->accept(this);
		auto currentType = dynamic_cast<Type*>(get<0>(it)->getSymbol());
		get<0>(it)->setSymbol(currentType);
		functionSymbol->addArgumentType(currentType);
		functionType->addArgumentType(currentType);

		if(get<1>(it)){

			auto variable = new VariableSymbol(currentType, get<1>(it)->getSimpleName());
			variable->setPosition(get<1>(it)->getPosition());
			get<1>(it)->setSymbol(variable);
			functionScope->declareVariable(variable);
		}
	 	// if(get<2>(it)){
	 	// 	get<2>(it)->accept(this);
	 	// }
	}	

	if(node->isVarargs()){
		functionType->setVarargs();
		functionSymbol->setVarargs();
	}


	currentScope->declareFunction(functionSymbol);
	if(node->isConstruct()){
		if(functionSymbol->getArgumentTypes().size() == 1){
			returnType->addConversion(functionSymbol->getArgumentTypes()[0], functionSymbol);
		}
	}

	//node->setFunctionSymbol(functionSymbol);
	node->setSymbol(functionSymbol);
	restoreCurrentScope();
}

void TypeVisitor::visit(FunctionDefinitionNode *node){
 	node->getSignature()->accept(this);
 	node->getSignature()->setSymbol(currentScope->resolveFunction(node->getSignature()->getName()));

 	this->scopes.push(currentScope);
 	node->getSignature()->getFunctionSymbol()->define(node->getSignature()->getPosition());
 	currentScope = node->getSignature()->getFunctionSymbol()->getFunctionScope();
 	node->getOperators()->accept(this);
 	currentScope = this->scopes.top();
 	this->scopes.pop();
}

void TypeVisitor::visit(IfNode *node){
 	

 	
 	node->getCondition()->accept(this);

 	LocalScope *thenScope = new LocalScope(currentScope, "thenscope " + node->getPosition().toString());
 	node->getThenBranch()->setScope(thenScope);
 	currentScope->declareAnonymousScope(thenScope);
 	setCurrentScope(thenScope);
 	node->getThenBranch()->accept(this);
 	restoreCurrentScope();

 	if(node->getElseBranch()){
 		LocalScope *elseScope = new LocalScope(currentScope, "elsescope " + node->getPosition().toString());
 		node->getElseBranch()->setScope(elseScope);
 		currentScope->declareAnonymousScope(elseScope);
 		setCurrentScope(elseScope);
 		node->getElseBranch()->accept(this);
 		restoreCurrentScope();
 	}

}

void TypeVisitor::visit(WhileNode *node){

	node->getCondition()->accept(this);

 	LocalScope *loopScope = new LocalScope(currentScope, "whilescope " + node->getPosition().toString());
 	node->getLoop()->setScope(loopScope);
 	currentScope->declareAnonymousScope(loopScope);
 	setCurrentScope(loopScope);
 	node->getLoop()->accept(this);
 	restoreCurrentScope();

 	if(node->getElseBranch()){
 		LocalScope *elseScope = new LocalScope(currentScope, "elsescope " + node->getPosition().toString());
 		node->getElseBranch()->setScope(elseScope);
 		currentScope->declareAnonymousScope(elseScope);
 		setCurrentScope(elseScope);
 		node->getElseBranch()->accept(this);
 		restoreCurrentScope();
 	}
}

void TypeVisitor::visit(ForNode *node){

	LocalScope *loopScope = new LocalScope(currentScope, "forLoopScope " + node->getPosition().toString());
 	node->setScope(loopScope);	
 	currentScope->declareAnonymousScope(loopScope);
 	setCurrentScope(loopScope);

 	if(node->getInit()){
 		node->getInit()->accept(this);
 	}

 	if(node->getCondition()){
 		node->getCondition()->accept(this);
 	}

 	if(node->getStep()){
 		node->getStep()->accept(this);
 	}

 	if(node->getAction()){
 		node->getAction()->setScope(loopScope);
 		node->getAction()->accept(this);
 	}

 	restoreCurrentScope();
}



void TypeVisitor::visit(ReturnNode *node){

 	if(node->getResult()){
 		node->getResult()->accept(this);
 	}
}

void TypeVisitor::visit(UsingNode *node){

 	if(dynamic_cast<SignatureNode*>(node->getType())){
 		auto signature = dynamic_cast<SignatureNode*>(node->getType());
 		CompoundNameNode *fullName = signature->getName();
		string functionTypeName = fullName->getSimpleName();

		FunctionType *functionType = new FunctionType;
		setCurrentScope(currentScope->resolveNamedScope(fullName));

		signature->getType()->accept(this);
		Type* returnType = dynamic_cast<Type*>(signature->getType()->getSymbol());
		signature->getType()->setSymbol(functionType);

		functionType->setReturnType(returnType);

		for(auto it: signature->getArguments()){
			get<0>(it)->accept(this);
			auto currentType = dynamic_cast<Type*>(get<0>(it)->getSymbol());
			get<0>(it)->setSymbol(currentType);
			functionType->addArgumentType(currentType);
		}	

		if(signature->isVarargs()){
			functionType->setVarargs();
		}

		functionType->setName(functionTypeName);
		currentScope->declareType(functionType);
		node->setSymbol(functionType);

		restoreCurrentScope();
 	}
 	else{
 		//auto type = dynamic_cast<TypeNode*>(node->getType());
 		node->getType()->accept(this);
 		auto fullName = dynamic_cast<CompoundNameNode*>(node->getName());
 		string typeName = fullName->getSimpleName();

 		setCurrentScope(currentScope->resolveNamedScope(fullName));

 		//auto typeToDeclare = new Type(*currentScope->resolveModifiedType(type));
 		//typeToDeclare->setName(typeName);
 		//currentScope->declareType(typeToDeclare);

 		auto typeToDeclare = dynamic_cast<Type*>(node->getType()->getSymbol());
 		//cout << "Going to declare " << typeToDeclare->toString() << " as " << typeName << "\n";
 		currentScope->declareType(typeName, typeToDeclare);
 		node->setSymbol(typeToDeclare);

 		restoreCurrentScope();

 		//cout << "=======\n";
 	}
 	
}

void TypeVisitor::visit(StructNode *node){


 	Type *type = new Type(node->getName()->getSimpleName()); 	
 	AbstractScope *parentScope = currentScope->resolveNamedScope(node->getName());
 	StructureScope *structureScope = new StructureScope(parentScope);
 	StructureSymbol *structure = new StructureSymbol(type, structureScope);
 	setCurrentScope(parentScope);
 	//currentScope->declareType(type);
 	currentScope->declareStructure(structure);
 	structure = currentScope->resolveStructure(structure->getName());
 	//currentScope->declareNamedScope(structure->getStructureScope());
 	setCurrentScope(structure->getStructureScope());

 	node->setSymbol(structure);

 	bool isDefinition = false;
 	for(auto it: node->getStructures()){
 		it->accept(this);
 		isDefinition = true;
 	}

 	for(auto it: node->getVariables()){
 		it->accept(this);
 		isDefinition = true;
 	}

 	for(auto it: node->getFunctions()){
 		it->accept(this);
 		isDefinition = true;
 	}


 	if(isDefinition){
 		structure->define(node->getPosition());
 	}
 	else {
 		structure->setPosition(node->getPosition());
 	}
 	
 	restoreCurrentScope();
 	restoreCurrentScope();
}

TypeVisitor::TypeVisitor(AbstractScope* scope){
	this->globalScope = scope;
	currentScope = this->globalScope;
}

TypeVisitor::TypeVisitor(){
	this->globalScope = new GlobalScope();
	currentScope = globalScope;
}

TypeVisitor::~TypeVisitor(){

}

void TypeVisitor::dump(ostream *out){
	currentScope->dump(out);
}

void TypeVisitor::setCurrentScope(AbstractScope* scope){
	this->scopes.push(currentScope);
	currentScope = scope;
}

void TypeVisitor::restoreCurrentScope(){
	currentScope = this->scopes.top();
	this->scopes.pop();
}

BasicNode* TypeVisitor::insertConversion(BasicNode* node, FunctionSymbol *functionSymbol){
	if(!functionSymbol){
		return node;
	}
	auto *result = new FunctionCallNode();
	result->addArgument(node);
	auto *functionCall = new FunctionCallSymbol();

	functionCall->setFunction(functionSymbol);
	result->setSymbol(functionCall);
	return result;
}

AbstractScope* TypeVisitor::getGlobalScope(){
	return this->globalScope;
}

void TypeVisitor::setGlobalScope(AbstractScope *scope){
	delete this->globalScope;
	this->globalScope = scope;
}

// void TypeVisitor::autoimport(){
// 	import(currentScope, "autoimport");
// }
