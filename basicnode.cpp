#include "basicnode.hpp"

string BasicNode::getText(){
	return this->text;
}

Position BasicNode::getPosition(){
	return this->position;
}

void BasicNode::setPosition(Position position){
	this->position = position;
}


BasicNode::~BasicNode(){}

BasicNode::BasicNode(){}
BasicNode::BasicNode(Position position){
	this->position = position;
}