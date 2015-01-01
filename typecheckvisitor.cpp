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
	//node->getType()->accept(this);
	Type *type = this->currentScope->resolveType(Type(node->getType()));

	for(auto it: node->getVariables()){
		//it.first->accept(this);
		currentScope->declareVariable(VariableSymbol(type, it.first->getNames()[0]));

		// if(it.second){
		// 	it.second->accept(this);
		// }
	}	

}

void TypeVisitor::visit(SignatureNode *node){

	CompoundNameNode *name = node->getName();
	string functionName = name->getSimpleName();


	setCurrentScope(currentScope->resolveNamedScope(name));

	Type* returnType = currentScope->resolveType(node->getType());

	FunctionSymbol function;
	function.setReturnType(returnType);

	FunctionScope *functionScope = new FunctionScope(currentScope, functionName);

//	functionScope->setParentScope(currentScope);

	functionScope->setReturnType(returnType);
	function.setFunctionScope(functionScope);

	function.setName(node->getName()->getNames()[0]);
	functionScope->setName(functionName);
	node->setFunctionScope(functionScope);

	for(auto it: node->getArguments()){
		function.addArgument(currentScope->resolveType(get<0>(it)));
		if(get<1>(it)){
			functionScope->declareVariable(VariableSymbol(currentScope->resolveType(get<0>(it)), get<1>(it)->getSimpleName()));
		}
	 	// if(get<2>(it)){
	 	// 	get<2>(it)->accept(this);
	 	// }
	}	

	currentScope->declareFunction(function);

	restoreCurrentScope();
}

void TypeVisitor::visit(FunctionDefinitionNode *node){
 	node->getSignature()->accept(this);

 	this->scopes.push(currentScope);
 	currentScope = node->getSignature()->getFunctionScope();
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
 	node->getName()->accept(this);

 	for(auto it: node->getFunctions()){
 		it->accept(this);
 	}

 	for(auto it: node->getVariables()){
 		it->accept(this);
 	}
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