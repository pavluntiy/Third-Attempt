#include "typecheckvisitor.hpp"

void TypeVisitor::visit(ProgramNode *node){
	if(node->getChild()){
		node->getChild()->accept(this);
	}
}

void TypeVisitor::visit(OperatorsNode *node){
	for(auto it : node->getChildren()){
		it->accept(this);
	}
}

void TypeVisitor::visit(CompoundNameNode *node){
}

void TypeVisitor::visit(FunctionCallNode *node){
	for(auto it: node->getFunctionArgs()){
		it->accept(this);
	}
}

void TypeVisitor::visit(TypeNode *node){
	for(auto it: node->getDimensions()){
		it->accept(this);
	}
}

void TypeVisitor::visit(ValueNode *node){
	delete node;
	node = nullptr;
}

void TypeVisitor::visit(VarDeclarationNode *node){
	

	Type *type = this->currentScope->resolveType(new Type(node->getType()));

	for(auto it: node->getVariables()){
		//it.first->accept(this);
		CompoundNameNode *name = it.first;
		string varName = name->getSimpleName();
		setCurrentScope(currentScope->resolveNamedScope(name));
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


	setCurrentScope(currentScope->resolveNamedScope(name));

	Type* returnType = currentScope->resolveType(Type(node->getType()));

	FunctionSymbol *function = new FunctionSymbol();
	function->setReturnType(returnType);
	function->setPosition(node->getPosition());

	FunctionScope *functionScope = new FunctionScope(currentScope, functionName);

	functionScope->setReturnType(returnType);
	function->setFunctionScope(functionScope);

	function->setName(functionName);
	functionScope->setName(functionName);
	node->setFunctionSymbol(function);

	for(auto it: node->getArguments()){
		function->addArgument(currentScope->resolveType(Type(get<0>(it))));
		if(get<1>(it)){
			auto variable = new VariableSymbol(currentScope->resolveType(get<0>(it)), get<1>(it)->getSimpleName());
			variable->setPosition(get<1>(it)->getPosition());
			functionScope->declareVariable(variable);
		}
	 	// if(get<2>(it)){
	 	// 	get<2>(it)->accept(this);
	 	// }
	}	

	currentScope->declareFunction(function);
	node->setFunctionSymbol(function);
	restoreCurrentScope();
}

void TypeVisitor::visit(FunctionDefinitionNode *node){
 	node->getSignature()->accept(this);
 	node->getSignature()->setFunctionSymbol(currentScope->resolveFunction(node->getSignature()->getName()));

 	this->scopes.push(currentScope);
 	node->getSignature()->getFunctionSymbol()->define(node->getSignature()->getPosition());
 	currentScope = node->getSignature()->getFunctionSymbol()->getFunctionScope();
 	node->getOperators()->accept(this);
 	currentScope = this->scopes.top();
 	this->scopes.pop();
}

void TypeVisitor::visit(IfNode *node){
 	
 	node->getCondition()->accept(this);

 	node->getThenBranch()->accept(this);

 	if(node->getElseBranch()){
 		node->getElseBranch()->accept(this);
 	}
}

void TypeVisitor::visit(WhileNode *node){

 	node->getCondition()->accept(this);

 	node->getLoop()->accept(this);

 	if(node->getElseBranch()){
 		node->getElseBranch()->accept(this);
 	}
}

void TypeVisitor::visit(ForNode *node){

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
 		node->getAction()->accept(this);
 	}
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
 	structure->setPosition(node->getPosition());
 	//currentScope->declareNamedScope(structure->getStructureScope());
 	setCurrentScope(structure->getStructureScope());

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
 	
 	restoreCurrentScope();
 	restoreCurrentScope();
}

TypeVisitor::TypeVisitor(){
	currentScope = &globalScope;
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