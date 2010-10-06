/*
 *  Node.cpp
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-05.
 *
 */

#include "Node.h"

//Constructor
Node::Node()
{
	//this->children = new vector<Node*>;
	
	this->scale = new Vector3();
	this->rotate = new Vector3();
	this->translate = new Vector3();
}

//Destructor
Node::~Node()
{
	//delete this->children;
	delete this->scale;
	delete this->rotate;
	delete this->translate;
}

//Add a child
void Node::addChild(Node* child)
{
	this->children->push_back(child);
}