#include "Mesh.h"

//Material Mesh
MaterialMesh::MaterialMesh(){
	
}

MaterialMesh::~MaterialMesh(){
	ResetMesh();
}

void MaterialMesh::ResetMesh(){
	delete[] this->vertices;
	this->vertices = NULL;
	this->TotalVertexCount = 0;
	glDeleteBuffers(1, &this->VBO);
	this->VBO = 0;
	glDeleteVertexArrays(1, &this->VAO);
	this->VAO = 0;
}

void MaterialMesh::AddVertex(glm::vec3 VertexCoordinates){
	VertexNoTexture *verticesBUF = new VertexNoTexture[this->TotalVertexCount + 1];
	for (unsigned int i = 0; i < this->TotalVertexCount; ++i)
		verticesBUF[i] = this->vertices[i];
	delete[] vertices;
	this->vertices = verticesBUF;
	this->vertices[this->TotalVertexCount] = VertexNoTexture(VertexCoordinates);
	++this->TotalVertexCount;
}

unsigned long int MaterialMesh::GenerateVAO(GLuint &VAO){
	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	this->VAO = VAO;
	
	if (this->VBO != 0)
		glDeleteBuffers(1, &this->VBO);
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNoTexture)*TotalVertexCount, vertices, GL_STATIC_DRAW);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNoTexture), (GLvoid*)offsetof(VertexNoTexture, position));
    glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
	
	return TotalVertexCount;
}

//Other types...