/*
 *  PolyMesh.cpp
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-05.
 *
 */

#include "PolyMeshNode.h"

//Constructor
PolyMeshNode::PolyMeshNode() : Node()
{
	this->mesh = NULL;
	this->texture = NULL;
}

//Destructor
PolyMeshNode::~PolyMeshNode()
{
	delete this->mesh;
	delete this->texture;
	delete this;
}

//attachModel
void PolyMeshNode::attachModel(OBJModel* model)
{
	this->mesh = model;
}

//attachTexture
void PolyMeshNode::attachTexture(Texture* texture)
{
	this->texture = texture;
}

void PolyMeshNode::draw()
{

	//Load texture
	if (this->texture != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, this->texture->textureID);
	}

	//Check vertex buffer object extension
	
	if (glewGetExtension("GL_ARB_vertex_buffer_object"))
	{
		//Load vertices
		glBindBuffer(GL_ARRAY_BUFFER, this->mesh->vbo_vertices);
		glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_VERTEX_ARRAY);

		//Load normals
		glBindBuffer(GL_ARRAY_BUFFER, this->mesh->vbo_normals);
		glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_NORMAL_ARRAY);

		//Load UVs
		glBindBuffer(GL_ARRAY_BUFFER, this->mesh->vbo_uvs);
		glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mesh->vbo_index);

		glDrawElements(GL_TRIANGLES, 3*this->mesh->numFaces, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	} else {
	

		//Drawing with vertex arrays
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, this->mesh->vertexArray);
		glNormalPointer(GL_FLOAT, 0, this->mesh->normalsArray);
		glTexCoordPointer(2, GL_FLOAT, 0, this->mesh->uvArray);

		glDrawElements(GL_TRIANGLES, 3*this->mesh->numFaces, GL_UNSIGNED_INT, this->mesh->indexArray);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}


/*

	//Old immediate mode drawing code

	glBegin(GL_TRIANGLES);
	for (int face = 0; face < this->numFaces; face++)
	{

		//Draw the vertex at each of the vertex numbders
		glTexCoord2f((GLfloat)this->uvs[this->uvIndex[face][0]]->u, (GLfloat)this->uvs[this->uvIndex[face][0]]->v);
		glNormal3f((GLfloat)this->normals[this->normalsIndex[face][0]]->x, (GLfloat)this->normals[this->normalsIndex[face][0]]->y, (GLfloat)this->normals[this->normalsIndex[face][0]]->z);
		glVertex3f((GLfloat)this->vertices[this->index[face][0]]->x, (GLfloat)this->vertices[this->index[face][0]]->y, (GLfloat)this->vertices[this->index[face][0]]->z);

		glTexCoord2f((GLfloat)this->uvs[this->uvIndex[face][1]]->u, (GLfloat)this->uvs[this->uvIndex[face][1]]->v);
		glNormal3f((GLfloat)this->normals[this->normalsIndex[face][1]]->x, (GLfloat)this->normals[this->normalsIndex[face][1]]->y, (GLfloat)this->normals[this->normalsIndex[face][1]]->z);
		glVertex3f((GLfloat)this->vertices[this->index[face][1]]->x, (GLfloat)this->vertices[this->index[face][1]]->y, (GLfloat)this->vertices[this->index[face][1]]->z);

		glTexCoord2f((GLfloat)this->uvs[this->uvIndex[face][2]]->u, (GLfloat)this->uvs[this->uvIndex[face][2]]->v);
		glNormal3f((GLfloat)this->normals[this->normalsIndex[face][2]]->x, (GLfloat)this->normals[this->normalsIndex[face][2]]->y, (GLfloat)this->normals[this->normalsIndex[face][2]]->z);
		glVertex3f((GLfloat)this->vertices[this->index[face][2]]->x, (GLfloat)this->vertices[this->index[face][2]]->y, (GLfloat)this->vertices[this->index[face][2]]->z);

		/* Non-Optimized code for debugging
		//Get vertex numbers
		int v1, v2, v3;
		v1 = this->index[face][0];
		v2 = this->index[face][1];
		v3 = this->index[face][2];

		//Get Normal numbers
		int n1, n2, n3;
		n1 = this->normalsIndex[face][0];
		n2 = this->normalsIndex[face][1];
		n3 = this->normalsIndex[face][2];

		//Get UVs
		int uv1, uv2, uv3;
		uv1 = this->uvIndex[face][0];
		uv2 = this->uvIndex[face][1];
		uv3 = this->uvIndex[face][2];

		//Draw the vertex at each of the vertex numbders
		glTexCoord2f((GLfloat)this->uvs[uv1]->u, (GLfloat)this->uvs[uv1]->v);
		glNormal3f((GLfloat)this->normals[n1]->x, (GLfloat)this->normals[n1]->y, (GLfloat)this->normals[n1]->z);
		glVertex3f((GLfloat)this->vertices[v1]->x, (GLfloat)this->vertices[v1]->y, (GLfloat)this->vertices[v1]->z);

		glTexCoord2f((GLfloat)this->uvs[uv2]->u, (GLfloat)this->uvs[uv2]->v);
		glNormal3f((GLfloat)this->normals[n2]->x, (GLfloat)this->normals[n2]->y, (GLfloat)this->normals[n2]->z);
		glVertex3f((GLfloat)this->vertices[v2]->x, (GLfloat)this->vertices[v2]->y, (GLfloat)this->vertices[v2]->z);

		glTexCoord2f((GLfloat)this->uvs[uv3]->u, (GLfloat)this->uvs[uv3]->v);
		glNormal3f((GLfloat)this->normals[n3]->x, (GLfloat)this->normals[n3]->y, (GLfloat)this->normals[n3]->z);
		glVertex3f((GLfloat)this->vertices[v3]->x, (GLfloat)this->vertices[v3]->y, (GLfloat)this->vertices[v3]->z);
		*/
	/*
	}
	glEnd();
	*/

}

void PolyMeshNode::render()
{

	
	//Draw this
	glPushMatrix();
		glTranslatef((GLfloat)this->translate->x, (GLfloat)this->translate->y, (GLfloat)this->translate->z);

		//This piece of code is not good because the order of rotation is actually important. 
		//We need to figure out a way to rotate an entire local coordinate system.
		//This should be relatively simple but we'll need to figure it out... 
		glRotatef((GLfloat)this->rotate->z, (GLfloat)0.0f, (GLfloat)0.0f, (GLfloat)1.0f);
		glRotatef((GLfloat)this->rotate->y, (GLfloat)0.0f, (GLfloat)1.0f, (GLfloat)0.0f);
		glRotatef((GLfloat)this->rotate->x, (GLfloat)1.0f, (GLfloat)0.0f, (GLfloat)0.0f);

		glScalef((GLfloat)this->scale->x, (GLfloat)this->scale->y, (GLfloat)this->scale->z);
		
		this->draw();

		//Draw children
		for (int i=0; i < this->children->size(); i++)
		{
			this->children->at(i)->render();
		}

	glPopMatrix();	
}