#include "deletevisitor.hpp"

void DeleteVisitor::visit(ProgramNode *node){
	if(node->getChild()){
		node->getChild()->accept(this);
	}
	delete node;
	node = nullptr;
}

void DeleteVisitor::visit(BlockNode *node){
	if(node->getChild()){
		node->getChild()->accept(this);
	}
	delete node;
	node = nullptr;
}



void DeleteVisitor::visit(OperatorsNode *node){
	for(auto it : node->getChildren()){
		it->accept(this);
	}
	delete node;
	node = nullptr;
}

void DeleteVisitor::visit(CompoundNameNode *node){
	delete node;
	node = nullptr;
}

void DeleteVisitor::visit(FunctionCallNode *node){
	for(auto it: node->getFunctionArgs()){
		it->accept(this);
	}
	delete node;
	node = nullptr;
}

void DeleteVisitor::visit(TypeNode *node){
	for(auto it: node->getDimensions()){
		it->accept(this);
	}
	
	delete node;
	node = nullptr;
}

void DeleteVisitor::visit(ValueNode *node){
	delete node;
	node = nullptr;
}

void DeleteVisitor::visit(VarDeclarationNode *node){
	node->getType()->accept(this);
	for(auto it: node->getVariables()){
		it.first->accept(this);
		if(it.second){
			it.second->accept(this);
		}
	}	
	delete node;
	node = nullptr;
}

void DeleteVisitor::visit(SignatureNode *node){
 	node->getType()->accept(this);
 	node->getName()->accept(this);

	for(auto it: node->getArguments()){
		get<0>(it)->accept(this);
		if(get<1>(it)){
			get<1>(it)->accept(this);
		}
	 	if(get<2>(it)){
	 		get<2>(it)->accept(this);
	 	}
	}	
	delete node;
	node = nullptr;
}

void DeleteVisitor::visit(FunctionDefinitionNode *node){
 	node->getSignature()->accept(this);
 	node->getOperators()->accept(this);
	delete node;
	node = nullptr;
}

void DeleteVisitor::visit(IfNode *node){
 	
 	node->getCondition()->accept(this);

 	node->getThenBranch()->accept(this);

 	if(node->getElseBranch()){
 		node->getElseBranch()->accept(this);
 	}
 	delete node;
 	node = nullptr;
}

void DeleteVisitor::visit(WhileNode *node){

 	node->getCondition()->accept(this);

 	node->getLoop()->accept(this);

 	if(node->getElseBranch()){
 		node->getElseBranch()->accept(this);
 	}
 	delete node;
 	node = nullptr;
}

void DeleteVisitor::visit(ForNode *node){

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
 	delete node;
 	node = nullptr;
}

void DeleteVisitor::visit(ReturnNode *node){

 	if(node->getResult()){
 		node->getResult()->accept(this);
 	}

	delete node;
	node = nullptr;
}

void DeleteVisitor::visit(StructNode *node){
 	node->getName()->accept(this);

 	for(auto it: node->getFunctions()){
 		it->accept(this);
 	}

 	for(auto it: node->getVariables()){
 		it->accept(this);
 	}

 	for(auto it: node->getStructures()){
 		it->accept(this);
 	}

 	delete node;
 	node = nullptr;
}
DeleteVisitor::~DeleteVisitor(){

}