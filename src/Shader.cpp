#include "Shader.h"
Shader::Shader(std::string vertexFile, std::string fragmentFile, std::string geometryFile) {
    this->Logging = false;
	this->ShaderLogger = NULL;
	std::string vertexSrc = OpenShaderSource(vertexFile);
    std::string fragmentSrc = OpenShaderSource(fragmentFile);
    std::string geometrySrc = "";
    if (geometryFile != "")
        geometrySrc = OpenShaderSource(geometryFile);
    if ((vertexSrc != "ERR") && (fragmentSrc != "ERR") && (geometrySrc != "ERR"))
        this->ID = CreateShader(vertexSrc, fragmentSrc, geometrySrc);
    else
        this->ID = 0;
}

Shader::Shader(std::string vertexFile, std::string fragmentFile, std::string geometryFile, Logger *ShaderLogger) {
    if (ShaderLogger != NULL){
		this->ShaderLogger = ShaderLogger;
		this->Logging = true;
		this->ShaderLogger->AddToLog("Started loading shader...");
	} else {
		this->Logging = false;
		this->ShaderLogger = NULL;
	}
	if (this->Logging)
		this->ShaderLogger->AddToLog("Loading vertex shader file...");
	std::string vertexSrc = OpenShaderSource(vertexFile);
	if (this->Logging)
		this->ShaderLogger->AddToLog("Loading fragment shader file...");
    std::string fragmentSrc = OpenShaderSource(fragmentFile);
    std::string geometrySrc = "";
    if (geometryFile != ""){
		if (this->Logging)
			this->ShaderLogger->AddToLog("Loading geometry shader file...");
        geometrySrc = OpenShaderSource(geometryFile);
	} else {
		if (this->Logging)
			this->ShaderLogger->AddToLog("No geometry shader specified. Skipping...");
	}
    if ((vertexSrc != "ERR") && (fragmentSrc != "ERR") && (geometrySrc != "ERR")){
		if (this->Logging)
			this->ShaderLogger->AddToLog("Started creating program...");
        this->ID = CreateShader(vertexSrc, fragmentSrc, geometrySrc);
	}
    else
        this->ID = 0;
}

Shader::~Shader() {
    glDeleteProgram(this->ID);
    ID = 0;
}

std::string Shader::OpenShaderSource(const std::string filepath){
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss;
    if (!stream.is_open()){
        if (this->Logging)
			this->ShaderLogger->AddToLog("Failed to load shader from file \"" + filepath + "\".");
        return "ERR";
    }
    while (getline(stream, line)){
        ss << line << "\n";
    }
    stream.close();
    return ss.str();
}

GLuint Shader::CompileShader(GLenum type, const std::string source){
    GLuint ShaderID = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(ShaderID, 1, &src, NULL);
    glCompileShader(ShaderID);
    int res;
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE){
        int length;
        glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(ShaderID, length, &length, message);
        if (this->Logging){
			this->ShaderLogger->AddToLog("Failed to compile shader");
			this->ShaderLogger->AddToLog("Shader info log:\n", message);
		}
        glDeleteShader(ShaderID);
        return 0;
    }
	if (this->Logging)
		this->ShaderLogger->AddToLog("Compiled successfully");
    return ShaderID;
}

GLuint Shader::CreateShader(std::string vertexShader, std::string fragmentShader, std::string geometryShader){
    GLuint program = glCreateProgram();
    if (this->Logging)
		this->ShaderLogger->AddToLog("Compiling vertex shader...");
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	if (this->Logging)
		this->ShaderLogger->AddToLog("Compiling fragment shader...");
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    GLuint gs = 0;
    if (geometryShader != ""){
		if (this->Logging)
			this->ShaderLogger->AddToLog("Compiling geometry shader...");
        gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
	} else {
		if (this->Logging)
			this->ShaderLogger->AddToLog("Geometry shader not specified. Skipping...");
	}
    bool geometryOK = (geometryShader == "") || ((geometryShader != "") && (gs != 0));
    if ((vs == 0) || (fs == 0) || (!geometryOK)){
		if (this->Logging)
			this->ShaderLogger->AddToLog("Shader program creation failed");
        glDeleteProgram(program);
        return 0;
    }
	if (this->Logging)
		this->ShaderLogger->AddToLog("Attaching vertex shader");
    glAttachShader(program, vs);
    if (geometryShader != ""){
		if (this->Logging)
			this->ShaderLogger->AddToLog("Attaching geometry shader");
        glAttachShader(program, gs);
	}
	if (this->Logging)
		this->ShaderLogger->AddToLog("Attaching fragment shader");
    glAttachShader(program, fs);
	if (this->Logging)
		this->ShaderLogger->AddToLog("Linking shader program");
    glLinkProgram(program);
	GLint LinkingResults;
	glGetProgramiv(program, GL_LINK_STATUS, &LinkingResults);
	if (LinkingResults == GL_FALSE){
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar *infoLog = new GLchar[maxLength];
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);
		if (this->Logging){
			this->ShaderLogger->AddToLog("Shader program linking failed");
			this->ShaderLogger->AddToLog("Linking info log:\n", infoLog);
			this->ShaderLogger->AddToLog("Shader program creation failed");
		}
		glDeleteProgram(program);
		return 0;		
	} 
	if (this->Logging){
		this->ShaderLogger->AddToLog("Shader program linked succefully");
		this->ShaderLogger->AddToLog("Validating shader program");
	}
    glValidateProgram(program);
	GLint ValidationResults;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &ValidationResults);
	if (ValidationResults == GL_FALSE){
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar *infoLog = new GLchar[maxLength];
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);
		if (this->Logging){
			this->ShaderLogger->AddToLog("Shader program validation failed");
			this->ShaderLogger->AddToLog("Validation info log:\n", infoLog);
			this->ShaderLogger->AddToLog("Shader program creation failed");
		}
		glDeleteProgram(program);
		return 0;		
	}
	if (this->Logging)
		this->ShaderLogger->AddToLog("Shader program validated successfully");
    glDetachShader(program, vs);
    if (geometryShader != "")
        glDetachShader(program, gs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    if (geometryShader != "")
        glDeleteShader(gs);
    glDeleteShader(fs);
	return program;
}

void Shader::SetUsed(bool activate){
    if (activate)
        glUseProgram(this->ID);
    else
        glUseProgram(0);
}

void Shader::SetUniform1i(std::string name, int value){
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::SetUniform2i(std::string name, glm::ivec2 value){
    glUniform2iv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetUniform3i(std::string name, glm::ivec3 value){
    glUniform3iv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetUniform4i(std::string name, glm::ivec4 value){
    glUniform4iv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetUniform1f(std::string name, float value){
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::SetUniform2f(std::string name, glm::fvec2 value){
    glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetUniform3f(std::string name, glm::fvec3 value){
    glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetUniform4f(std::string name, glm::fvec4 value){
    glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetUniformMat3(std::string name, glm::mat3 value, GLboolean transpose = GL_FALSE){
    glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), 1, transpose, glm::value_ptr(value));
}

void Shader::SetUniformMat4(std::string name, glm::mat4 value, GLboolean transpose = GL_FALSE){
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, transpose, glm::value_ptr(value));
}

