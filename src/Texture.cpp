#include "Texture.h"

Texture::Texture(std::string path, GLenum TextureType) {
    this->filePath = path;
    unsigned char* localBuffer = NULL;
    this->width = 0;
    this->height = 0;
    this->bitsPerPixel = 0;
    this->TextureType = TextureType;
    stbi_set_flip_vertically_on_load(1);
    if (TextureType == GL_TEXTURE_2D){
        localBuffer = stbi_load(path.c_str(), &this->width, &this->height, &this->bitsPerPixel, 4);
        if (!localBuffer)
            _Exit(-1);
        glGenTextures(1, &this->ID);
        glBindTexture(TextureType, this->ID);
        glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(TextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(TextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(TextureType, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
        glBindTexture(TextureType, 0);
    } else if (TextureType == GL_TEXTURE_CUBE_MAP){
        glGenTextures(1, &this->ID);
        glBindTexture(TextureType, this->ID);
        glTexParameteri(TextureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(TextureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(TextureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(TextureType, GL_TEXTURE_BASE_LEVEL, 0); 
        glTexParameteri(TextureType, GL_TEXTURE_MAX_LEVEL, 0); 
        for (GLuint i = 0; i < 6; ++i){
            std::string name;
            switch (i){
                case 0:{
                    name = "_RIGHT";
                    break;
                }
                case 1:{
                    name = "_LEFT";
                    break;
                }
                case 2:{
                    name = "_TOP";
                    break;
                }
                case 3:{
                    name = "_BOTTOM";
                    break;
                }
                case 4:{
                    name = "_BACK";
                    break;
                }
                case 5:{
                    name = "_FRONT";
                    break;
                }
                default: break;
            }
            name = this->filePath /*+ "texture"*/ + name;
            localBuffer = stbi_load(name.c_str(), &this->width, &this->height, &this->bitsPerPixel, 0);
            if (!localBuffer)
                _Exit(-1);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
        }
        glBindTexture(TextureType, 0);
    }
    
    if (localBuffer)
        stbi_image_free(localBuffer);
}

Texture::Texture(std::string path, GLenum TextureType, Logger *TextureLogger){
	if (TextureLogger != NULL){
		this->Logging = true;
		this->TextureLogger = TextureLogger;
		this->TextureLogger->AddToLog("Started loading texture");
	} else {
		this->Logging = false;
		this->TextureLogger = NULL;
	}
	this->filePath = path;
    unsigned char* localBuffer = NULL;
    this->width = 0;
    this->height = 0;
    this->bitsPerPixel = 0;
    this->TextureType = TextureType;
    stbi_set_flip_vertically_on_load(1);
    if (TextureType == GL_TEXTURE_2D){
        localBuffer = stbi_load(path.c_str(), &this->width, &this->height, &this->bitsPerPixel, 4);
        if (!localBuffer){
            if (this->Logging){
				this->TextureLogger->AddToLog("Error loading file \"" + this->filePath + "\"");
				this->TextureLogger->AddToLog("Terminating...");
			}
            _Exit(-1);
        }
        glGenTextures(1, &this->ID);
        glBindTexture(TextureType, this->ID);
        glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(TextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(TextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(TextureType, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
        glBindTexture(TextureType, 0);
    } else if (TextureType == GL_TEXTURE_CUBE_MAP){
        glGenTextures(1, &this->ID);
        glBindTexture(TextureType, this->ID);
        glTexParameteri(TextureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(TextureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(TextureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(TextureType, GL_TEXTURE_BASE_LEVEL, 0); 
        glTexParameteri(TextureType, GL_TEXTURE_MAX_LEVEL, 0); 
        for (GLuint i = 0; i < 6; ++i){
            std::string name;
            switch (i){
                case 0:{
                    name = "_RIGHT";
                    break;
                }
                case 1:{
                    name = "_LEFT";
                    break;
                }
                case 2:{
                    name = "_TOP";
                    break;
                }
                case 3:{
                    name = "_BOTTOM";
                    break;
                }
                case 4:{
                    name = "_BACK";
                    break;
                }
                case 5:{
                    name = "_FRONT";
                    break;
                }
                default: break;
            }
            name = this->filePath /*+ "texture"*/ + name;
            localBuffer = stbi_load(name.c_str(), &this->width, &this->height, &this->bitsPerPixel, 0);
            if (!localBuffer){
				if (this->Logging){
					this->TextureLogger->AddToLog("Error loading file \"" + name + "\"");
					this->TextureLogger->AddToLog("Terminating...");
				}
                _Exit(-1);
            }
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
        }
        glBindTexture(TextureType, 0);
    }
    if (this->Logging)
		this->TextureLogger->AddToLog("Texture loaded successfully");
    if (localBuffer)
        stbi_image_free(localBuffer);
}

Texture::~Texture() {
    glDeleteTextures(1, &this->ID);
}

void Texture::Bind(GLuint slot = 0){
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(this->TextureType, this->ID);
    this->slot = slot;
}

void Texture::Unbind(){
    glBindTexture(this->TextureType, 0);
    this->slot = 0;
}

