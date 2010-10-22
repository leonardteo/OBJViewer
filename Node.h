/*
 *  Node.h
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-05.
 *
 */

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

#include <GL/glew.h>

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/GLUT.h>
#else
	#include <GL/glut.h>
#endif

#include "Vector3.h"

using namespace std;

enum NodeType {
	POLYMESH, CAMERA, LIGHT, TRANSFORM
};

class Node
{
public:

	//Data Members
	NodeType type;				//The type of node this is
	vector<Node*>* children;	//Child nodes
	Node* parent;				//The node's parent
	string id;					//The node's ID so that we can search for a node
	
	//Local Transforms
	Vector3* scale;				//Local scaling	
	Vector3* rotate;			//Local rotation on x, y, z axes
	Vector3* translate;			//Local translation

	//Methods
	Node();
	~Node();
	void addChild(Node*);
	void setParent(Node*);
	virtual void render();
	virtual void viewTransform();
	
};

#endif
