/**
My attempt at an OBJ parser
Leonard Teo

Done:
Only supports triangles - remember to triangulate all models

To DO:
- Generate normals if a model doesn't have them
- Handle groups and materials

**/

#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/GLUT.h>
#else
	#include <GL/glut.h>
#endif

#include "Vector3.h"
#include "MyBitmap.h"
#include "Node.h"
#include <vector>

using namespace std;

struct UV 
{
	float u;
	float v;
};

class OBJModel: public Node
{
public:
	//Constructor
	OBJModel(const char* fileName);

	//Destructor
	~OBJModel(void);

	//Load Texture
	void loadTexture(const char* fileName);

	//Draw the polygons using OpenGL calls
	void draw();
	void drawArray();
	
	//Scene graph rendering call
	void render();

	//Generate normals - TO DO
	void generateNormals();

private:

	//Private data members
	//Total number of vertices
	int numVertices;
	int numFaces;
	int numUVs;
	int numNormals;
	
	bool hasTexture;

	//Data
	Vector3** vertices;
	Vector3** normals;
	UV** uvs;
	int** index;			//2 dimensional arrays: Note these are pointers so that we can do this: this->normals_index[i][j]
	int** normalsIndex;
	int** uvIndex;

	GLfloat* vertexArray;	
	GLfloat* normalsArray;
	GLfloat* uvArray;

	GLuint* indexArray; //Straight line array

	//Texture map
	MyBitmap* image;
	GLuint texture;

	//Initialize the arrays and variables
	void init();

	//Load a model file
	void load(const char* fileName);

	//Prepare arrays
	void prepArrays();
	void debugArrays();
};

#endif