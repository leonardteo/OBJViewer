/*************************
Polygon Mesh data structure
TO IMPLEMENT
***************************/

#ifndef POLYMESH
#define POLYMESH

class PolyMesh
{
public:
	PolyMesh(void);
	~PolyMesh(void);

	//Total number of vertices
	int num_vertices;
	int num_faces;
	int num_uvs;
	int num_normals;

	//Arrays of data
	float* faces;
	float* vertices;
	float* normals;
	int* index;
	int* normals_index;
	float* uvs;

private:
	void init();

};

#endif