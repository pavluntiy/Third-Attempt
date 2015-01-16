#include "typecheckvisitor.hpp"

void TypeVisitor::visit(ProgramNode *node){
	setCurrentScope(globalScope);
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

void TypeVisitor::visit(FunctionCallNode *node){

	//node->setSymbol(currentScope->resolveFunction( dynamic_cast<CompoundNameNode*>(node->getFunctionName())));//Provisional

	auto possibleFuctionArgument = node->getFunctionName();
	node->getFunctionName()->accept(this);
	auto functionCall = new FunctionCallSymbol();

	functionCall->setFullName(dynamic_cast<CompoundNameNode*>(node->getFunctionName()));

	for(auto it: node->getFunctionArgs()){
		it->accept(this);
		functionCall->addArgument(it->getSymbol()->getType());
	}
	functionCall->setFunction(currentScope->resolveFunctionCall(functionCall));

	if(functionCall->getConversions().size() > 0){
		auto conversions = functionCall->getConversions();
		auto arguments = node->getFunctionArgs();
		int bound = conversions.size();
		for(int i = 0; i < bound; ++i){
			if(conversions[i]){
				node->replaceArgument(i, insertConversion(arguments[i], conversions[i]));
			}
		}
	}

	node->setSymbol(functionCall);


	//node->setType(currentScope->resolveType("int"));
}

void TypeVisitor::visit(DotNode *node){
	node->getLeft()->accept(this);
	node->getRight()->accept(this);
}

void TypeVisitor::visit(ImportNode *node){


	auto names = node->getModuleName()->getNames();

	string name = ".";

	for(auto it: names){
		name += "/" + it;
	}

	auto in = new ifstream(name + ".cch");

	if(!in->is_open()){
		throw TypeException("Bad module", node->getPosition());
	}


	Lexer lexer(*in);

	Parser parser(lexer);
	try{
		parser.buildTree();
	}
	catch (ParserException &pe){

		throw ParserException( "In module '" + name + "' parse error:\n"  + pe.what() );
	}

	TypeVisitor *typeVisitor = new TypeVisitor();

	try{
		parser.getTree()->accept(typeVisitor);
	}
	catch (NoticeException &ne){
		cout << ne.what() << '\n';
	}
	catch (TypeException &te){
		throw TypeException("In module " + name + te.what(), node->getPosition());
	}

	node->setTree(parser.getTree());

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
	for(auto it: node->getDimensions()){
		it->accept(this);
	}
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

		// case ValueNode::Type::FLOAT: {
		// 	auto type = new CompoundNameNode("char");
		// 	node->setSymbol(currentScope->resolveType(type)); 
		// 	delete type;
		// }	
		// break;

		default: throw TypeException("What is it!?", node->getPosition());
	}
		
	node->setSymbol(valueSymbol); 
}

void TypeVisitor::visit(VarDeclarationNode *node){
	
	Type *type = this->currentScope->resolveModifiedType(Type(node->getType()));
	//cout <<"Nanana!";
	node->getType()->setSymbol(type);

	for(auto it: node->getVariables()){
		//it.first->accept(this);
		CompoundNameNode *name = it.first;
		string varName = name->getSimpleName();
		setCurrentScope(currentScope->resolveNamedScope(name));
		//cout << currentScope->getName() << '\n';
		auto variable = new VariableSymbol(type, varName);
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



	Type* returnType = currentScope->resolveModifiedType(Type(node->getType()));
	node->getType()->setSymbol(returnType);
	//node->getType()->setSymbol(functionType->getReturnType());
	functionType->setReturnType(returnType);

	//node->setType(functionType);

	FunctionSymbol *function = new FunctionSymbol();
	function->setFullName(node->getName());
	//function->setReturnType(returnType);
	function->setPosition(node->getPosition());

	function->setType(functionType);
	function->setReturnType(returnType);

	FunctionScope *functionScope = new FunctionScope(currentScope, functionName);

	//functionScope->setReturnType(returnType);
	function->setFunctionScope(functionScope);

	function->setName(functionName);
	functionScope->setName(functionName);
	node->setFunctionSymbol(function);

	for(auto it: node->getArguments()){
		auto currentType = currentScope->resolveModifiedType(Type(get<0>(it)));
		get<0>(it)->setSymbol(currentType);
		function->addArgument(currentType);
		functionType->addArgument(currentType);

		if(get<1>(it)){
			auto variable = new VariableSymbol(currentScope->resolveType(get<0>(it)), get<1>(it)->getSimpleName());
			variable->setPosition(get<1>(it)->getPosition());
			get<1>(it)->setSymbol(variable);
			functionScope->declareVariable(variable);
		}
	 	// if(get<2>(it)){
	 	// 	get<2>(it)->accept(this);
	 	// }
	}	

	if(node->isVarargs()){
		function->setVarargs();
	}


	currentScope->declareFunction(function);
	if(node->isConstruct()){
		if(function->getArguments().size() == 1){
			returnType->addConversion(function->getArguments()[0], function);
		}
	}

	//node->setFunctionSymbol(function);
	node->setSymbol(function);
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

 	for(auto it: node->getFunctions()){
 		it->accept(this);
 		isDefinition = true;
 	}

 	for(auto it: node->getVariables()){
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

TypeVisitor::TypeVisitor(){
	this->globalScope = new GlobalScope();
	currentScope = globalScope;
	this->globalScopeFound = false;
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

BasicNode* TypeVisitor::insertConversion(BasicNode* node, FunctionSymbol *function){
	if(!function){
		return node;
	}
	auto *result = new FunctionCallNode();
	result->addArgument(node);
	auto *functionCall = new FunctionCallSymbol();

	functionCall->setFunction(function);
	result->setSymbol(functionCall);
	return result;
}