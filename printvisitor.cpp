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
	// if(node->getChildren().size() > 0){
	//  	*this->out << shift;
	// }
	// else {
	// 	*this->out << ' ';
	// }
	*this->out << shift << ")" << endl;
	
}

void PrintVisitor::visit(CompoundNameNode *node){
	*this->out << shift << "( " << node->toString();
	*this->out << " '" << node->getLeft() << "' ";

	this->shift.push_back(' ');
	if(node->getRight()){
		cout << endl;
		node->getRight()->accept(this);
	}
	this->shift.pop_back();
	
	*this->out << shift  << ")" << endl;
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

		node->getName()->accept(this);

	this->shift.pop_back();
	
	*this->out << shift << ")" << endl;
}

void PrintVisitor::visit(ValueNode *node){
	*this->out << shift << "( " << node->toString();
	if(node->getText() != ""){
	 	*this->out<< " \"" << node->getText() << "\"";
	}
	*this->out   << shift << ")" << endl;
}

PrintVisitor::PrintVisitor(ostream *out){
		this->shift = "";
		this->out = out;
}