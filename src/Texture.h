#ifndef TEXTURE_H
#define TEXTURE_H
#include "../Dependencies/glad/include/glad/glad.h"
#include "../Dependencies/stb/include/stb_image.h"

#include "Logger.h"

#include <string>

class Texture {
public:
    Texture(std::string, GLenum);
	Texture(std::string, GLenum, Logger*);
    ~Texture();
    void Bind(GLuint);
    void Unbind();
    inline int GetWidth() { return width; }
    inline int GetHeight() { return height; }
    inline GLuint GetSlot() { return slot; }
private:
	Logger *TextureLogger;
	bool Logging;
    GLuint ID, slot;
    GLenum TextureType;
    std::string filePath;
    int width, height, bitsPerPixel;
};

#endif /* TEXTURE_H */

