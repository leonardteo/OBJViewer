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
	for (int i=0; i<this->numVertices; i++)
	{
		delete this->vertices[i];
	}
	delete this->vertices;
	
	for (int i=0; i<this->numNormals; i++)
	{
		delete this->normals[i];
	}
	delete this->normals;
	
	for (int i=0; i<this->numUVs; i++)
	{
		delete this->uvs[i];
	}
	delete this->uvs;
	
	delete this->index;
	delete this->normalsIndex;
	delete this->uvIndex;
}

//General class initialization
void OBJModel::init()
{
	this->numVertices = 0;
	this->numFaces = 0;
	this->numUVs = 0;

	this->numNormals = 0;
	this->vertices = NULL;
	this->uvs = NULL;
	this->index = NULL;
	this->normals = NULL;
	this->uvIndex = NULL;
	
	this->type = POLYMESH;	
	this->hasTexture = false;
}

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
		this->index = new int*[this->numFaces];			//Triangles now

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
				//sscanf(buffer, "f %d/%*d/%d %d/%*d/%d %d/%*d/%d", &this->index[currentIndex][0], &this->normalsIndex[currentNormalIndex][0], &this->index[currentIndex][1], &this->normalsIndex[currentNormalIndex][1], &this->index[currentIndex][2], &this->normalsIndex[currentNormalIndex][2]);
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

	
	//Debug - echo out all the vertices and faces
	/*
	printf("Vertices: \n");
	for(int i=0; i< this->numVertices; i++)
	{
		printf(" %f %f %f \n", this->vertices[i]->x, this->vertices[i]->y, this->vertices[i]->z );
	}

	printf("\n\nUV coordinates: \n");
	for (int i=0; i< this->numUVs; i++)
	{
		printf(" %f %f \n", this->uvs[i]->u, this->uvs[i]->v);
	}

	printf("\n\nVertex Normals: \n");
	for(int i=0; i< this->numNormals ; i++)
	{
		printf(" %f %f %f \n", this->normals[i]->x, this->normals[i]->y, this->normals[i]->z);
	}
	

	printf("\n\nIndex/UV/Normal: \n");
	for(int i=0; i< this->numFaces; i++){
		printf(" %d/%d/%d %d/%d/%d %d/%d/%d\n", this->index[i][0], this->uvIndex[i][0], this->normalsIndex[i][0], this->index[i][1], this->uvIndex[i][1], this->normalsIndex[i][1], this->index[i][2], this->uvIndex[i][2], this->normalsIndex[i][2]);
	}
	*/
	

	//decrement all indices to cater for OpenGL (OBJ indices start at 1)
	for(int i = 0; i < this->numFaces; i++)
	{
		for(int j=0; j<3; j++){
			this->index[i][j]--;
			this->normalsIndex[i][j]--;
			this->uvIndex[i][j]--;
		}
	}


}


void OBJModel::render()
{
	for (int i=0; i<this->children->size(); i++)
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
		
	//For each face, draw a Triangle
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

	}
	glEnd();
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