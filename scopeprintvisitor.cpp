#include "scopeprintvisitor.hpp"

void ScopePrintVisitor::visit(ProgramNode *node){
	setCurrentScope(node->getScope());
	if(node->getAutoImport()){
		this->shift.push_back('\t');
		node->getAutoImport()->accept(this);
		this->shift.pop_back();
	}
	if(node->getChild()){
		this->shift.push_back('\t');
		node->getChild()->accept(this);
		this->shift.pop_back();
	}
	restoreCurrentScope();
}

void ScopePrintVisitor::visit(BlockNode *node){
	setCurrentScope(node->getScope());
	currentScope->dump(out, shift);
	if(node->getChild()){
		this->shift.push_back('\t');
		node->getChild()->accept(this);
		this->shift.pop_back();
	}
	restoreCurrentScope();
}

void ScopePrintVisitor::visit(OperatorsNode *node){
	this->shift.push_back('\t');
	for(auto it : node->getChildren()){
		it->accept(this);
	}
	this->shift.pop_back();
}

void ScopePrintVisitor::visit(CompoundNameNode *node){

	*this->out << shift << node->getSymbol()->toString() << endl;
	// *this->out << shift << "( " << node->toString();

	// if(node->getNames().size() > 0){
	// 	*this->out << ' ';
	// }

	// if(node->getNames().size() > 1){
	// 	*this->out << endl << shift;
	// }

	// for(auto it: node->getNames()){
	// 	*this->out << "'" <<  it << "' ";
	// }
	// if(node->getNames().size() > 1){
	// 	*this->out << endl;
	// }
	
	// if(node->getNames().size() > 1){
	// 	*this->out << shift;
	// }

	// *this->out  << ")" << endl;
}

void ScopePrintVisitor::visit(FunctionCallNode *node){
	// cout << node->getSymbol()->getType()->toString();
	// return;

	*this->out << shift << "(" << node->toString() << endl;
	this->shift.push_back('\t');
	*this->out << shift << node->getSymbol()->toString() << endl;
	// if(node->getFunctionName()){
	// 	node->getFunctionName()->accept(this);
	// }
	for(auto it: node->getFunctionArgs()){
		it->accept(this);
	}
	this->shift.pop_back();
	
	*this->out << shift << ")" << endl;
}

void ScopePrintVisitor::visit(DotNode *node){
	*this->out << shift << "( " << node->toString() << endl;
	node->getLeft()->accept(this);
	node->getRight()->accept(this);
	*this->out << shift << ")\n";
}

void ScopePrintVisitor::visit(ImportNode *node){


	*this->out << shift << "( " << node->toString() << endl;
		
	//dynamic_cast<ImportSymbol*>(node->getSymbol())->getScope()->dump(out, shift);
		node->getTree()->accept(this);
	this->shift.push_back(' ');

	*this->out   << shift << ")" << endl;
}

void ScopePrintVisitor::visit(TypeNode *node){
	if(node->getSymbol()){
		*this->out << shift << node->getSymbol()->toString();
	}
	// *this->out << shift << "( " << node->toString() << '\n';

	// this->shift.push_back(' ');
	// *this->out << this->shift;
	// for(auto it: node->getStorageModes()){
	// 	*this->out << it << ' ';
	// }
	// *this->out << endl;
	// *this->out << this->shift;
	// for(auto it: node->getModifiers()){
	// 	*this->out << it << ' ';
	// }
	// *this->out << endl;
	// *this->out << this->shift;
	// for(auto it: node->getAccessModes()){
	// 	*this->out << it << ' ';
	// }
	// *this->out << endl;

	// if(node->getDimensions().size() > 0){
	// 	*this->out << shift << "ARRAY:\n";
	// 	for(auto it: node->getDimensions()){
	// 		it->accept(this);
	// 	}
	// }

	// node->getName()->accept(this);

	// this->shift.pop_back();
	
	//*this->out << shift << ")" << endl;
}

void ScopePrintVisitor::visit(ValueNode *node){
	
	*this->out << shift << "( " << node->getSymbol()->toString();
	if(node->getText() != ""){
	 	*this->out<< " \"" << node->getText() << "\"";
	}
	*this->out  << " )" << endl;
}

void ScopePrintVisitor::visit(VarDeclarationNode *node){
	*this->out << shift << "( " << node->toString() << endl;
		
	this->shift.push_back('\t');
	//node->getType()->accept(this);
	for(auto it: node->getVariables()){
		it.first->accept(this);
		if(it.second){
			it.second->accept(this);
		}
	}	

	this->shift.pop_back();

	*this->out << "\n"  << shift << ")" << endl;
}

void ScopePrintVisitor::visit(SignatureNode *node){


	*this->out << shift << "(function " << node->getSymbol()->toString()<<  "\n";
		
	this->shift.push_back('\t');
	
 	//node->getType()->accept(this);
 	//node->getName()->accept(this);

 	if(node->isVarargs()){
 		*this->out << shift << "VARARGS\n";
 	}

	for(auto it: node->getArguments()){
		//get<0>(it)->accept(this);
		if(get<1>(it)){
			get<1>(it)->accept(this);
		}
		else{
			get<0>(it)->accept(this);
		}
		*this->out << endl;
	 	// if(get<2>(it)){
	 	// 	get<2>(it)->accept(this);
	 	// }
	}	

	this->shift.pop_back();

	*this->out   << shift << ")" << endl;
}

void ScopePrintVisitor::visit(FunctionDefinitionNode *node){


	*this->out << shift << "( " << node->toString() << endl;
		
	this->shift.push_back('\t');
 	
 	node->getSignature()->accept(this);
 	node->getOperators()->accept(this);

	this->shift.pop_back();

	*this->out   << shift << ")" << endl;
}

void ScopePrintVisitor::visit(IfNode *node){


	*this->out << shift << "( " << node->toString() << endl;
		
	this->shift.push_back('\t');
 	
 	node->getCondition()->accept(this);
 	node->getThenBranch()->getScope()->dump(out, shift);
 	node->getThenBranch()->accept(this);

 	if(node->getElseBranch()){
 		node->getElseBranch()->getScope()->dump(out, shift);
 		node->getElseBranch()->accept(this);
 	}

	this->shift.pop_back();

	*this->out   << shift << ")" << endl;
}

void ScopePrintVisitor::visit(WhileNode *node){


	*this->out << shift << "( " << node->toString() << endl;
		
	this->shift.push_back('\t');
 	
 	node->getCondition()->accept(this);
 	node->getLoop()->getScope()->dump(out, shift);
 	node->getLoop()->accept(this);


 	if(node->getElseBranch()){
 		node->getElseBranch()->getScope()->dump(out, shift);
 		node->getElseBranch()->accept(this);
 	}

	this->shift.pop_back();

	*this->out   << shift << ")" << endl;
}

void ScopePrintVisitor::visit(ForNode *node){


	*this->out << shift << "(for:" << endl;
		
	this->shift.push_back('\t');
 	
	node->getScope()->dump(this->out, shift);

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

void ScopePrintVisitor::visit(ReturnNode *node){


	//*this->out << shift << "( " << node->toString() << endl;
		
	this->shift.push_back('\t');

 	if(node->getResult()){
 		node->getResult()->accept(this);
 	}

	this->shift.pop_back();

	//*this->out   << shift << ")" << endl;
}

void ScopePrintVisitor::visit(UsingNode *node){
		
	this->shift.push_back('\t');

 	*this->out << shift << "(Using " << node->getSymbol()->toString() << " as type )\n"; 

	this->shift.pop_back();
}

void ScopePrintVisitor::visit(StructNode *node){

	*this->out << shift << "(Structure " << node->getSymbol()->toString() <<": "<< endl;
	
	//dynamic_cast<StructureSymbol*>(node->getSymbol())->getStructureScope()->dump(this->out);
	this->shift.push_back('\t');


 	//node->getName()->accept(this);

 	//*this->out << shift << "Functions:\n";

 	for(auto it: node->getFunctions()){
 		it->accept(this);
 	}

 	//*this->out << "\n" << shift << "Variables:\n";

 	for(auto it: node->getVariables()){
 		it->accept(this);
 	}

 	//*this->out << "\n" << shift << "Structures:\n";

 	for(auto it: node->getStructures()){
 		it->accept(this);
 	}


	this->shift.pop_back();

	//*this->out   << shift << ")" << endl;

}

ScopePrintVisitor::ScopePrintVisitor(ostream *out){
	this->shift = "";
	this->out = out;
}

ScopePrintVisitor::~ScopePrintVisitor(){

}


void ScopePrintVisitor::setCurrentScope(AbstractScope* scope){
	this->scopes.push(currentScope);
	currentScope = scope;
}

void ScopePrintVisitor::restoreCurrentScope(){
	currentScope = this->scopes.top();
	this->scopes.pop();
}