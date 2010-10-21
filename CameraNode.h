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
#include "Vector3.h"

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

	//@todo: Should these be private? 

	//Members
	Vector3* point_of_interest;	//Where the camera should be looking at
	float twist;				//Roll
	float elevation;			//Pitch
	float azimuth;				//Heading
	float distance;				//Distance from point of interest
	float fov;					//Field of View
	float near_plane;			//Near clipping plane
	float far_plane;			//Far clipping plane

	bool perspective_mode;		//Switch between perspective and ortho

	//Ortho
	float ortho_zoom;			//The orthographic zoom factor

};

#endif
