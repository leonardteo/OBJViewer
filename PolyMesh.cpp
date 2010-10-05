#include "PolyMesh.h"
#include <iostream>

using namespace std;

PolyMesh::PolyMesh(void)
{
	this->init();
}

void PolyMesh::init()
{
	this->num_vertices = 0;
	this->num_faces = 0;
	this->num_normals = 0;
	this->vertices = NULL;
	this->index = NULL;
	this->normals = NULL;
}

PolyMesh::~PolyMesh(void)
{
}
