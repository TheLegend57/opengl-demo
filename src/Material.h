#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Dependencies/GLM/include/glm/glm.hpp"
#include <string>

struct Material{
public:
	std::string MaterialName;
	float SpecularExponent = 1.f;
	glm::vec3 AmbientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 DiffuseColor = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 SpecularColor = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 EmissionColor = glm::vec3(0.f, 0.f, 0.f);
	float OpticalDensity = 1.f;
	float Transparency = 0.f;
};

#endif /* MATERIAL_H */