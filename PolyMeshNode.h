/*
 *  PolyMesh.h
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-05.
 *
 */

#ifndef POLYMESHNODE_H
#define POLYMESHNODE_H

#include "Node.h"
#include "OBJModel.h"
#include "Texture.h"

#include <GL\glew.h>

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/GLUT.h>
#else
	#include <GL/glut.h>
#endif

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

class PolyMeshNode : public Node {

public:

	//Constructor/Destructor
	PolyMeshNode();
	~PolyMeshNode();

	//Attach model - note that this class does NOT load OBJModels. It attaches them so that you can have instances of models
	void attachModel(OBJModel* model);

	//Attach texture
	void attachTexture(Texture* texture);

	//Draw the polygons using OpenGL calls
	void draw();
	
	//Scene graph rendering call
	void render();

	//Data members
	OBJModel* mesh;
	Texture* texture;

};

#endif
