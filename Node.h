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
	vector<Node*>* children;		//Child nodes

	//Local Transforms
	Vector3* scale;				//Local scaling	
	Vector3* rotate;			//Local rotation on x, y, z axes
	Vector3* translate;			//Local translation

	//Methods
	Node();
	~Node();
	void addChild(Node*);
	virtual void render();
	
private:
	
};

#endif
