/**
Polar Camera Class
@author Leonard Teo
**/

#ifndef POLARCAMERA
#define POLARCAMERA

#include "Vector3.h"

class PolarCamera
{
public:

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


	//Methods
	PolarCamera(void);
	~PolarCamera(void);
};

#endif