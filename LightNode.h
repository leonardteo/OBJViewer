#ifndef LIGHTNODE_H
#define LIGHTNODE_H

#include "Node.h"

#include <GL/glew.h>

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/GLUT.h>
#else
	#include <GL/glut.h>
#endif

enum LightType{
	SPOTLIGHT, DIRECTIONAL, POINTLIGHT
};

class LightNode : public Node
{
public:
	LightNode(GLenum id, LightType type);
	~LightNode(void);

	LightType type;
	GLenum id;	

	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat position[4];
	GLfloat spotDirection[3];

	GLfloat spotCutOff;

	bool on;

	void render(RenderType renderType = ALL_OBJECTS);

};

#endif