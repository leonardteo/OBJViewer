#include "PolarCamera.h"


PolarCamera::PolarCamera(void)
{
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

PolarCamera::~PolarCamera(void)
{
	delete point_of_interest;
}
