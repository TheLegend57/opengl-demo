#ifndef SHADER_H
#define SHADER_H
#include "../Dependencies/glad/include/glad/glad.h"
#include "../Dependencies/GLM/include/glm/glm.hpp"
#include "../Dependencies/GLM/include/glm/gtc/type_ptr.hpp"

#include "Logger.h"

//#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
public:
    Shader(std::string, std::string, std::string);
	Shader(std::string, std::string, std::string, Logger*);
    ~Shader();
    void SetUniform1i(std::string, int);
    void SetUniform2i(std::string, glm::ivec2);
    void SetUniform3i(std::string, glm::ivec3);
    void SetUniform4i(std::string, glm::ivec4);
    void SetUniform1f(std::string, float);
    void SetUniform2f(std::string, glm::fvec2);
    void SetUniform3f(std::string, glm::fvec3);
    void SetUniform4f(std::string, glm::fvec4);
    void SetUniformMat3(std::string, glm::mat3, GLboolean);
    void SetUniformMat4(std::string, glm::mat4, GLboolean);
    void SetUsed(bool);
    GLuint ID = 0;
private:
    std::string OpenShaderSource(const std::string);
    GLuint CompileShader(unsigned int, std::string);
    GLuint CreateShader(std::string, std::string, std::string);
	
	Logger *ShaderLogger;
	bool Logging;
};

#endif /* SHADER_H */

