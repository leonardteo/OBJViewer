/*
 *  Camera.h
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-06.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#ifndef CAMERANODE_H
#define CAMERANODE_H

#include "Node.h"

enum CameraProjection {
	PERSPECTIVE, ORTHO
};

enum CameraType {
	POLAR, FIRSTPERSON
};


/**
 * Camera class extends Node
 */

class CameraNode : public Node 
{
public:
	
	//Data Members
	CameraProjection cameraProjection;
	CameraType cameraType;
	
	//Constructor/Destructor
	CameraNode();
	~CameraNode();
	
};

#endif
