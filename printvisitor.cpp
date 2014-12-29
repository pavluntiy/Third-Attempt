#include "printvisitor.hpp"

void PrintVisitor::visit(ProgramNode *node){
	*this->out << shift << "( " << node->toString();
	if(node->getChild()){
		cout << endl;
		this->shift.push_back(' ');
		node->getChild()->accept(this);
		this->shift.pop_back();
	}

	*this->out   << ")" << endl;	
}

void PrintVisitor::visit(OperatorsNode *node){
	*this->out << shift << "( " << node->toString();

	if(node->getText() != ""){
	 	*this->out<< " \"" << node->getText() << "\"";
	}

	 *this->out << ", children num = "<< node->getChildren().size() << ":" << endl;
	 this->shift.push_back(' ');
	for(auto it : node->getChildren()){
		it->accept(this);
	}
	this->shift.pop_back();
	*this->out << shift << ")" << endl;	
}

void PrintVisitor::visit(CompoundNameNode *node){
	*this->out << shift << "( " << node->toString();

	if(node->getNames().size() > 0){
		*this->out << ' ';
	}

	if(node->getNames().size() > 1){
		*this->out << endl << shift;
	}

	for(auto it: node->getNames()){
		*this->out << "'" <<  it << "' ";
	}
	if(node->getNames().size() > 1){
		*this->out << endl;
	}
	
	if(node->getNames().size() > 1){
		*this->out << shift;
	}

	*this->out  << ")" << endl;
}

void PrintVisitor::visit(FunctionCallNode *node){
	*this->out << shift << "( " << node->toString() << '\n';

	this->shift.push_back(' ');
	if(node->getFunctionName()){
		node->getFunctionName()->accept(this);
	}
	for(auto it: node->getFunctionArgs()){
		it->accept(this);
	}
	this->shift.pop_back();
	
	*this->out << shift << ")" << endl;
}

void PrintVisitor::visit(TypeNode *node){
	*this->out << shift << "( " << node->toString() << '\n';

	this->shift.push_back(' ');
	*this->out << this->shift;
	for(auto it: node->getStorageModes()){
		*this->out << it << ' ';
	}
	*this->out << endl;
	*this->out << this->shift;
	for(auto it: node->getModifiers()){
		*this->out << it << ' ';
	}
	*this->out << endl;
	*this->out << this->shift;
	for(auto it: node->getAccessModes()){
		*this->out << it << ' ';
	}
	*this->out << endl;

	if(node->getDimensions().size() > 0){
		*this->out << shift << "ARRAY:\n";
		for(auto it: node->getDimensions()){
			it->accept(this);
		}
	}

	node->getName()->accept(this);

	this->shift.pop_back();
	
	*this->out << shift << ")" << endl;
}

void PrintVisitor::visit(ValueNode *node){
	*this->out << shift << "( " << node->toString();
	if(node->getText() != ""){
	 	*this->out<< " \"" << node->getText() << "\"";
	}
	*this->out  << " )" << endl;
}

void PrintVisitor::visit(VarDeclarationNode *node){
	*this->out << shift << "( " << node->toString() << endl;
		
	this->shift.push_back(' ');
	node->getType()->accept(this);

	for(auto it: node->getVariables()){
		it.first->accept(this);
		if(it.second){
			it.second->accept(this);
		}
	}	

	this->shift.pop_back();

	*this->out   << shift << ")" << endl;
}

void PrintVisitor::visit(SignatureNode *node){


	*this->out << shift << "( " << node->toString() << endl;
		
	this->shift.push_back(' ');
 	node->getType()->accept(this);
 	node->getName()->accept(this);

 	if(node->getVarargs()){
 		*this->out << shift << "VARARGS\n";
 	}

	for(auto it: node->getArguments()){
		get<0>(it)->accept(this);
		if(get<1>(it)){
			get<1>(it)->accept(this);
		}
	 	if(get<2>(it)){
	 		get<2>(it)->accept(this);
	 	}
	}	

	this->shift.pop_back();

	*this->out   << shift << ")" << endl;
}

void PrintVisitor::visit(FunctionDefinitionNode *node){


	*this->out << shift << "( " << node->toString() << endl;
		
	this->shift.push_back(' ');
 	
 	node->getSignature()->accept(this);
 	node->getOperators()->accept(this);

	this->shift.pop_back();

	*this->out   << shift << ")" << endl;
}

void PrintVisitor::visit(IfNode *node){


	*this->out << shift << "( " << node->toString() << endl;
		
	this->shift.push_back(' ');
 	
 	node->getCondition()->accept(this);

 	node->getThenBranch()->accept(this);

 	if(node->getElseBranch()){
 		node->getElseBranch()->accept(this);
 	}

	this->shift.pop_back();

	*this->out   << shift << ")" << endl;
}

void PrintVisitor::visit(WhileNode *node){


	*this->out << shift << "( " << node->toString() << endl;
		
	this->shift.push_back(' ');
 	
 	node->getCondition()->accept(this);

 	node->getLoop()->accept(this);

 	if(node->getElseBranch()){
 		node->getElseBranch()->accept(this);
 	}

	this->shift.pop_back();

	*this->out   << shift << ")" << endl;
}

void PrintVisitor::visit(ForNode *node){


	*this->out << shift << "( " << node->toString() << endl;
		
	this->shift.push_back(' ');
 	
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

	this->shift.pop_back();

	*this->out   << shift << ")" << endl;
}

void PrintVisitor::visit(ReturnNode *node){


	*this->out << shift << "( " << node->toString() << endl;
		
	this->shift.push_back(' ');

 	if(node->getResult()){
 		node->getResult()->accept(this);
 	}

	this->shift.pop_back();

	*this->out   << shift << ")" << endl;
}


PrintVisitor::PrintVisitor(ostream *out){
		this->shift = "";
		this->out = out;
}