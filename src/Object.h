#ifndef OBJECT_H
#define OBJECT_H
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Camera.h"
#include "LightSource.h"
#include <fstream>
#include <vector>

class MaterialObject{
public:
	MaterialObject();
	~MaterialObject();
	void LoadObject(std::string);
	void SetUpShader(Shader*);
	void GenerateVAOs();
	void DrawObject(glm::mat4, Camera*, DirectionalLight);
	void ResetObject();
private:
	bool ObjectLoaded = false, ShaderSetUp = false, VAOsGenerated = false;
	unsigned int TotalSubParts = 0;
	unsigned int *VertexCounts = NULL;
	GLuint *VAOs = NULL;
	GLuint *VBOs = NULL;
	GLuint *EBOs = NULL;
	Material *materials = NULL;
	MaterialMesh *meshes = NULL;
	Shader *MaterialShader = NULL;
};
/*
class Texture2DObject{
public:
	Texture2DObject();
	~Texture2DObject;
};

class CubeMapObject{
public:
	CubeMapObject();
	~CubeMapObject();
};
*/
#endif //OBJECT_H