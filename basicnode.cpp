#include "basicnode.hpp"

string BasicNode::getText(){
	return this->text;
}

Position BasicNode::getPosition(){
	return this->position;
}


BasicNode::~BasicNode(){}