#include "OBJModel.h"

using namespace std;

//Constructor
OBJModel::OBJModel(const char* fileName)
{
	this->init();
	this->load(fileName);
}

//Free memory
OBJModel::~OBJModel(void)
{
	//Free all vertices and normals
	delete [] this->vertices;
	delete [] this->normals;
	delete [] this->uvs;
	
	delete [] this->index;
	delete [] this->normalsIndex;
	delete [] this->uvIndex;

	delete [] this->vertexArray;
	delete [] this->normalsArray;
	delete [] this->uvArray;
	delete [] this->indexArray;
}

//General class initialization
void OBJModel::init()
{
	//Set node type
	this->type = POLYMESH;	
	this->hasTexture = false;

	this->numVertices = 0;
	this->numFaces = 0;
	this->numUVs = 0;
	this->numNormals = 0;

	//Set all arrays to null
	this->vertices = NULL;
	this->uvs = NULL;
	this->normals = NULL;
	this->index = NULL;
	this->uvIndex = NULL;
	this->normalsIndex = NULL;
	this->vertexArray = NULL;
	this->normalsArray = NULL;
	this->uvArray = NULL;
	this->indexArray = NULL;
}

/**
* Load the OBJ Model into memory
**/

void OBJModel::load(const char* fileName)
{
	char buffer[255];

	//Open file for reading
	ifstream file;
	file.open(fileName);

	if (!file.is_open())
	{
		//If file isn't open
		cout << "Error loading file: " << fileName << endl;
		return;
	} else {

		//First pass of reading the file counts the data
		while ( !file.eof() ){

			//Read the line
			file.getline(buffer, 255);

			//Read the line
			switch(buffer[0]) {

			//Vertex
			case 'v':
				switch(buffer[1]) {
				case ' ':
					this->numVertices++;
					break;
				case 'n':
					this->numNormals++;
					break;
				case 't':
					this->numUVs++;
					break;
				}
				break;

			//Face
			case 'f':
				this->numFaces++;
				break;

			default:
				break;
			}

		} //end of file

		//Echo the stats
		printf("fileName: %s \n", fileName);
		printf("Vertices: %d \n", this->numVertices);
		printf("Vertex Normals: %d \n", this->numNormals);
		printf("Faces: %d \n", this->numFaces);
		printf("\n");

		//Allocate memory for data

		//Init vertices
		this->vertices = new Vector3*[this->numVertices];
		this->index = new int*[this->numFaces];

		//Init normals
		this->normals = new Vector3*[this->numNormals];
		this->normalsIndex = new int*[this->numFaces];	//Triangles
		
		//Init UV's
		this->uvs = new UV*[this->numUVs];
		this->uvIndex = new int*[this->numFaces];

		//Initialize each of the indices
		for (int i=0; i<this->numFaces; i++){
			this->index[i] = new int[3];
			this->normalsIndex[i] = new int[3];
			this->uvIndex[i] = new int[3];
			for (int j=0; j<3; j++){
				this->index[i][j] = 0;
				this->normalsIndex[i][j] = 0;
				this->uvIndex[i][j] = 0;
			}
		}

		//Second pass of file to load data
		file.clear();				//Clear eof status
		file.seekg(0, ios::beg);	//Move cursor to beginning

		int currentVertex = 0;
		int currentIndex = 0;
		int currentNormal = 0;
		int currentUV = 0;
		int currentUVIndex = 0;
		int currentNormalIndex = 0;

		while (!file.eof()){

			//Read line
			file.getline(buffer, 255);

			switch (buffer[0]){

			//Vertex
			case 'v':
				switch(buffer[1]){
				case ' ':
					//Create new vertex
					this->vertices[currentVertex] = new Vector3();
					sscanf(buffer, "v %f %f %f", &this->vertices[currentVertex]->x, &this->vertices[currentVertex]->y, &this->vertices[currentVertex]->z);
					currentVertex++;
					break;
				case 'n':
					//Create new vertex normal
					this->normals[currentNormal] = new Vector3();
					sscanf(buffer, "vn %f %f %f", &this->normals[currentNormal]->x, &this->normals[currentNormal]->y, &this->normals[currentNormal]->z);
					currentNormal++;
					break;
				case 't':
					//Create new UV
					this->uvs[currentUV] = new UV();
					sscanf(buffer, "vt %f %f", &this->uvs[currentUV]->u, &this->uvs[currentUV]->v);
					currentUV++;
					break;
				}
				break;

			case 'f':
				sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &this->index[currentIndex][0], &this->uvIndex[currentUVIndex][0], &this->normalsIndex[currentNormalIndex][0], &this->index[currentIndex][1], &this->uvIndex[currentUVIndex][1], &this->normalsIndex[currentNormalIndex][1], &this->index[currentIndex][2], &this->uvIndex[currentUVIndex][2], &this->normalsIndex[currentNormalIndex][2]);
				currentIndex++;
				currentNormalIndex++;
				currentUVIndex++;
				break;
			}

		} //end of file

		//Release file pointer
		file.close();
	} 

	//decrement all indices to cater for OpenGL (OBJ indices start at 1)
	for(int i = 0; i < this->numFaces; i++)
	{
		for(int j=0; j<3; j++){
			this->index[i][j]--;
			this->normalsIndex[i][j]--;
			this->uvIndex[i][j]--;
		}
	}

	//Prepare the arrays for drawing
	this->prepArrays();

}

/**
Takes the vertex, normals and texture coords and turns them into straight march arrays for OpenGL to render
**/
void OBJModel::prepArrays()
{

	//Initialize the arrays
	this->vertexArray = new GLfloat[this->numFaces *3 * 3];	//3 vertices to a face, 3 floats to a vertex
	this->normalsArray = new GLfloat[this->numFaces *3 * 3];
	this->indexArray = new GLuint[this->numFaces * 3];
	this->uvArray = new GLfloat[this->numFaces * 3 * 2];	//3 vertices to a face, 2 floats to a UV

	//Turn index into a straight march
	int numElements = this->numFaces * 3;
	for (int i=0; i<numElements; i++)
	{
		this->indexArray[i] = i;
	}

	//Figure out vertices, normals and UVs
	for (int face = 0; face < this->numFaces; face++)
	{

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

		//Element 1
		this->uvArray[6*face] = (GLfloat) this->uvs[uv1]->u;
		this->uvArray[6*face + 1] = (GLfloat) this->uvs[uv1]->v;

		this->normalsArray[9*face] = (GLfloat) this->normals[n1]->x;
		this->normalsArray[9*face + 1] = (GLfloat) this->normals[n1]->y;
		this->normalsArray[9*face + 2] = (GLfloat) this->normals[n1]->z;

		this->vertexArray[9*face] = (GLfloat) this->vertices[v1]->x;
		this->vertexArray[9*face + 1] = (GLfloat) this->vertices[v1]->y;
		this->vertexArray[9*face + 2] = (GLfloat) this->vertices[v1]->z;

		//Element 2
		this->uvArray[6*face + 2] = (GLfloat) this->uvs[uv2]->u;
		this->uvArray[6*face + 3] = (GLfloat) this->uvs[uv2]->v;

		this->normalsArray[9*face + 3] = (GLfloat) this->normals[n2]->x;
		this->normalsArray[9*face + 4] = (GLfloat) this->normals[n2]->y;
		this->normalsArray[9*face + 5] = (GLfloat) this->normals[n2]->z;

		this->vertexArray[9*face + 3] = (GLfloat) this->vertices[v2]->x;
		this->vertexArray[9*face + 4] = (GLfloat) this->vertices[v2]->y;
		this->vertexArray[9*face + 5] = (GLfloat) this->vertices[v2]->z;

		//Element 3
		this->uvArray[6*face + 4] = (GLfloat) this->uvs[uv3]->u;
		this->uvArray[6*face + 5] = (GLfloat) this->uvs[uv3]->v;

		this->normalsArray[9*face + 6] = (GLfloat)this->normals[n3]->x;
		this->normalsArray[9*face + 7] = (GLfloat)this->normals[n3]->y;
		this->normalsArray[9*face + 8] = (GLfloat)this->normals[n3]->z;

		this->vertexArray[9*face + 6] = (GLfloat)this->vertices[v3]->x;
		this->vertexArray[9*face + 7] = (GLfloat)this->vertices[v3]->y;
		this->vertexArray[9*face + 8] = (GLfloat)this->vertices[v3]->z;	
	}

	//Release all the object memory - we only need the arrays
	delete [] this->vertices;
	delete [] this->normals;
	delete [] this->uvs;
	
	delete [] this->index;
	delete [] this->normalsIndex;
	delete [] this->uvIndex;

	//this->debugArrays();
}

//Show all the array data
void OBJModel::debugArrays()
{
	FILE *file = fopen("debug.txt", "w");
	if (file == NULL)
	{
		printf("Couldn't open debug file.");
		return;
	}
	
	fprintf(file, "Element Index: \n");
	for (int face=0; face < this->numFaces; face++)
	{
		fprintf(file, "%d %d %d \n", this->indexArray[3*face], this->indexArray[3*face + 1], this->indexArray[3*face + 2]);
	}

	fprintf(file, "\nVertices: \n");
	for (int face=0; face < this->numFaces; face++)
	{
		fprintf(file, "%f %f %f   %f %f %f   %f %f %f   \n", 
			this->vertexArray[3*face], this->vertexArray[3*face + 1], this->vertexArray[3*face + 2],
			this->vertexArray[3*face+3], this->vertexArray[3*face + 4], this->vertexArray[3*face + 5],
			this->vertexArray[3*face+6], this->vertexArray[3*face + 7], this->vertexArray[3*face + 8]
		);
	}

	fprintf(file, "\nNormals: \n");
	for (int face=0; face < this->numFaces; face++)
	{
		fprintf(file, "%f %f %f   %f %f %f   %f %f %f   \n", 
			this->normalsArray[3*face], this->normalsArray[3*face + 1], this->normalsArray[3*face + 2],
			this->normalsArray[3*face+3], this->normalsArray[3*face + 4], this->normalsArray[3*face + 5],
			this->normalsArray[3*face+6], this->normalsArray[3*face + 7], this->normalsArray[3*face + 8]
		);
	}

	fclose(file);
}


/**
Overloaded scene graph render routine
**/
void OBJModel::render()
{
	for (int i=0; i < this->children->size(); i++)
	{
		this->children->at(i)->render();
	}
	
	//Draw this
	glPushMatrix();
		glTranslatef((GLfloat)this->translate->x, (GLfloat)this->translate->y, (GLfloat)this->translate->z);
		this->draw();
	glPopMatrix();
	
}

/**
Drawing routine
**/
void OBJModel::draw()
{
	//Load texture
	if (this->hasTexture)
	{
		glBindTexture(GL_TEXTURE_2D, this->texture);
	}
		
	//Drawing with vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, this->vertexArray);
	glNormalPointer(GL_FLOAT, 0, this->normalsArray);
	glTexCoordPointer(2, GL_FLOAT, 0, this->uvArray);

	glDrawElements(GL_TRIANGLES, 3*this->numFaces, GL_UNSIGNED_INT, this->indexArray);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	//For each face, draw a Triangle
	
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

	glFlush();
}

/***
Load Texture
***/
void OBJModel::loadTexture(const char* fileName)
{
	this->image = new MyBitmap(fileName);
	if (!image->loaded){
		cout << "Could not load texture." << endl;
		exit(0);
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &this->texture);				//Allocate Texture Name
	glBindTexture(GL_TEXTURE_2D, this->texture);	//Select current texture

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->image->width, this->image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->image->data);
	this->hasTexture = true;
	
	glEnable(GL_TEXTURE_2D);
	

	delete image;
}