#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include "../Dependencies/GLM/include/glm/glm.hpp"

struct DirectionalLight{
	glm::vec3 Direction;
	float Strength;
	glm::vec3 LightColor;
};

#endif //LIGHTSOURCE_H