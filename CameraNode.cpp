/*
 *  Camera.cpp
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-06.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#include "CameraNode.h"

CameraNode::CameraNode(CameraType cameraType) : Node()
{
	//Set node type
	this->type = CAMERA;
	this->cameraType = cameraType;

	//Polar camera settings
	this->azimuth = 0.0;
	this->elevation = 0.0;
	this->twist = 0.0;
	this->distance = 0.0;
	this->panX = 0.0;
	this->panY = 0.0;

	this->azimuth_offset = 0.0;
	this->elevation_offset = 0.0;
	this->twist_offset = 0.0;
	this->distance_offset = 0.0;
	this->panX_offset = 0.0;
	this->panY_offset = 0.0;

	//Default Projection settings
	this->nearPlane = 0.1f;
	this->farPlane = 500.0f;
	this->fov = 45;
	this->perspectiveMode = true;

	this->orthoZoom = 2;

}

CameraNode::~CameraNode()
{
	delete this;
}

/**
Sets up a projection for the camera
Must provide viewport width and height
**/
void CameraNode::setProjection(int width, int height)
{
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Perspective mode or ortho
	float aspect_ratio = (float)width / (float)height;

	if (this->perspectiveMode){
		//If perspective mode, set up a perspective projection
		gluPerspective(this->fov, GLfloat(aspect_ratio), this->nearPlane, this->farPlane);

	} else {
		//If orthographic mode, set up an orthographic projection

		//Keep aspect ratio
		float left, right, bottom, top;

		if (aspect_ratio < 1.0){
			left = -this->orthoZoom;
			right = this->orthoZoom;
			bottom = -this->orthoZoom * ( 1.0f / aspect_ratio );
			top = this->orthoZoom * ( 1.0f / aspect_ratio );
		} else {
			left = -this->orthoZoom * aspect_ratio;
			right = this->orthoZoom * aspect_ratio;
			bottom = -this->orthoZoom;
			top = this->orthoZoom;
		}
		glOrtho(left, right, bottom, top, -this->farPlane, this->farPlane);
	}
}

/**
View transformations
**/
void CameraNode::viewTransform()
{
	//If first person
	if (this->cameraType == FIRSTPERSON){
		//To do: Make this rotate on a local, orthonormal basis
		glRotatef(this->rotate->z, 0.0f, 0.0f, 1.0f);	//Roll
		glRotatef(this->rotate->y, 0.0f, 1.0f, 0.0f);	//Pitch
		glRotatef(this->rotate->x, 1.0f, 0.0f, 0.0f);	//Heading
		glTranslatef(-this->translate->x, -this->translate->y, -this->translate->z);

	} else
	{
		//If third person
		glTranslatef(-((GLfloat)this->panX + (GLfloat)this->panX_offset), -((GLfloat)this->panY + (GLfloat)this->panY_offset), -( (GLfloat)this->distance + (GLfloat)this->distance_offset) );		//Translate along the z axis away from camera
		glRotatef((GLfloat)this->twist + (GLfloat)this->twist_offset, (GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0);							//Rotate around z axis (usually by 0)
		glRotatef((GLfloat)this->elevation + (GLfloat)this->elevation_offset, (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0);					//Rotate around x axis
		glRotatef((GLfloat)this->azimuth + (GLfloat)this->azimuth_offset, (GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0);						//Rotate around y axis

		//To do: Make this rotate on a local, orthonormal basis
		glRotatef(this->rotate->z, 0.0f, 0.0f, 1.0f);	//Roll
		glRotatef(this->rotate->y, 0.0f, 1.0f, 0.0f);	//Pitch
		glRotatef(this->rotate->x, 1.0f, 0.0f, 0.0f);	//Heading
		glTranslatef(-this->translate->x, -this->translate->y, -this->translate->z);
	}



	//Call parent viewTransform
	if (this->parent != NULL)
	{
		this->parent->viewTransform();
	}

}