#ifndef TEXT_H
#define TEXT_H
#include "Shader.h"
#include "Font.h"
class Text{
public:
	Text();
	~Text();
	void ResetText();
private:
	std::u32string *TextStrings;
	Font *TextFont;
	Shader *TextShader;
	unsigned int StringCount;
	GLuint VAO, VBO, EBO;
};
#endif