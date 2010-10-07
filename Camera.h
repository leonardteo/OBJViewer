/*
 *  Camera.h
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-06.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#include "Node.h"

enum CameraProjection {
	PERSPECTIVE, ORTHO
};

enum CameraType {
	POLAR, FIRSTPERSON
};

class Camera: public Node 
{
public:
	
	//Data Members
	CameraProjection cameraProjection;
	CameraType cameraType;
	
	//Constructor/Destructor
	Camera();
	~Camera();
	
};