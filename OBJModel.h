/**
My attempt at an OBJ parser
Leonard Teo

25 September 2010

Done:
Only supports triangles - remember to triangulate all models

To DO:
- UV mapping
- Generate normals if a model doesn't have them
- Handle groups and materials

**/

#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <GL\glut.h>
#include "Vector3.h"
#include "MyBitmap.h"

struct UV 
{
	float u;
	float v;
};

class OBJModel
{
public:
	//Constructor
	OBJModel(char* fileName);

	//Destructor
	~OBJModel(void);

	//Load Texture
	void loadTexture(char* fileName);

	//Draw the polygons using OpenGL calls
	void draw();

	//Generate normals - TO DO
	void generateNormals();

private:

	//Private data members
	//Total number of vertices
	int numVertices;
	int numFaces;
	int numUVs;
	int numNormals;

	//Data
	Vector3** vertices;
	Vector3** normals;
	UV** uvs;
	int** index;			//2 dimensional arrays: Note these are pointers so that we can do this: this->normals_index[i][j]
	int** normalsIndex;
	int** uvIndex;

	//Texture map
	MyBitmap* image;
	GLuint texture;

	//Initialize the arrays and variables
	void init();

	//Load a model file
	void load(char* fileName);

};

#endif