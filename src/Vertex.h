#ifndef VERTEX_H
#define VERTEX_H
#include "../Dependencies/GLM/include/glm/glm.hpp"
class Vertex {
public:
	Vertex();
    Vertex(glm::vec3, glm::vec2);
    ~Vertex();
    glm::vec3 position;
    glm::vec2 texcoord;
};

class VertexCubeMap {
public:
	VertexCubeMap();
    VertexCubeMap(glm::vec3, glm::vec3);
    ~VertexCubeMap();
    glm::vec3 position;
    glm::vec3 texcoord;
};

class VertexNoTexture {
public:
	VertexNoTexture();
    VertexNoTexture(glm::vec3);
    ~VertexNoTexture();
    glm::vec3 position;    
};
#endif /* VERTEX_H */

