/*
 *  Transform.h
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-10.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Node.h"

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#else
	#include <GL/glut.h>
#endif

enum TransformType {
	TRANSLATE, SCALE, ROTATE
};	

class Transform : public Node 
{
	
public:
	//Data members
	TransformType transformType;
	
	//Methods
	Transform(enum TransformType);
	~Transform();
	void render();
	
};

#endif
