#ifndef TEXTURE_H
#define TEXTURE_H

/**
Texture class for holding any textures
**/

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/GLUT.h>
#else
	#include <GL/glut.h>
#endif

#include "MyBitmap.h"

class Texture
{
public:

	//Constructor/Destructor
	Texture(void);
	Texture(const char* filename);
	~Texture(void);

	//Bitmap
	MyBitmap* textureMap;
	GLuint textureID;

private:
	void loadTexture(const char* filename);

};


#endif