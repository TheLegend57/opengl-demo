#ifndef MESH_H
#define MESH_H
#include "../Dependencies/glad/include/glad/glad.h"
#include "Vertex.h"

//DEBUG
#include <iostream>
//DEBUG

class MaterialMesh{
public:
	MaterialMesh();
	~MaterialMesh();
	void ResetMesh();
	unsigned long int GenerateVAO(GLuint&);
	VertexNoTexture *vertices = NULL;
	unsigned long int TotalVertexCount = 0;
	void AddVertex(glm::vec3);
private:
	GLuint VAO = 0, VBO = 0;
};

/*class Texture2DMesh{
public:
	Texture2DMesh();
	~Texture2DMesh();
	void ResetMesh();
private:
	Vertex *vertices = NULL;
	
};

class CubeMapMesh{
public:
	Texture2DMesh();
	~Texture2DMesh();
	void ResetMesh();
private:
	VertexCubeMap *vertices = NULL;
	GLuint *indices = NULL;
};*/
#endif /* MESH_H */