#include "Vertex.h"

//Regular Vertex
Vertex::Vertex(){
	this->position = glm::vec3(0.f, 0.f, 0.f);
    this->texcoord = glm::vec2(0.f, 0.f);
}

Vertex::Vertex(glm::vec3 position, glm::vec2 texcoord) {
    this->position = position;
    this->texcoord = texcoord;
}

Vertex::~Vertex() {
}

//Vertex for cubemap use
VertexCubeMap::VertexCubeMap(){
	this->position = glm::vec3(0.f, 0.f, 0.f);
    this->texcoord = glm::vec3(0.f, 0.f, 0.f);
}

VertexCubeMap::VertexCubeMap(glm::vec3 position, glm::vec3 texcoord) {
    this->position = position;
    this->texcoord = texcoord;
}

VertexCubeMap::~VertexCubeMap() {
}

//Vertex for use without textures
VertexNoTexture::VertexNoTexture(){
	this->position = glm::vec3(0.f, 0.f, 0.f);
}

VertexNoTexture::VertexNoTexture(glm::vec3 position) {
    this->position = position;
}

VertexNoTexture::~VertexNoTexture() {
}

