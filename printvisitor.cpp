#include "printvisitor.hpp"

void PrintVisitor::visit(ProgramNode *node){
	*this->out << shift << "( " << node->toString() << endl;

	this->shift.push_back(' ');
	node->getChild()->accept(this);
	this->shift.pop_back();

	*this->out   << ")" << endl;
	
}

void PrintVisitor::visit(ExpressionNode *node){
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
	if(node->getChildren().size() > 0){
	 	*this->out << shift;
	}
	else {
		*this->out << ' ';
	}
	*this->out   << ")" << endl;
	
}

void PrintVisitor::visit(ValueNode *node){
	*this->out << shift << "( " << node->toString();
	if(node->getText() != ""){
	 	*this->out<< " \"" << node->getText() << "\"";
	}
	*this->out   << ")" << endl;
}

PrintVisitor::PrintVisitor(ostream *out){
		this->shift = "";
		this->out = out;
}