/*
 *  Camera.cpp
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-06.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#include "CameraNode.h"

CameraNode::CameraNode()
{
	//Set node type
	this->type = CAMERA;

	//Polar camera settings
	this->point_of_interest = new Vector3(0.0, 0.0, 0.0);
	this->azimuth = -45;
	this->elevation = 30;
	this->twist = 0;
	this->distance = 15;
	this->near_plane = 0.1f;
	this->far_plane = 100;
	this->fov = 45;
	this->perspective_mode = true;
	this->ortho_zoom = 2;
}

CameraNode::~CameraNode()
{
}
