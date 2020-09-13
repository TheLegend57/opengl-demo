#ifndef FONT_H
#define FONT_H
#include "../Dependencies/GLM/include/glm/glm.hpp"
#include "Texture.h"
#include "Vertex.h"
#include "Logger.h"

const int SymbolsPerLine = 50;
const int LineCount = 4;

const std::u32string Symbols = U".!?,<>/\\|][()~_$;:'\"+-=*^#%{}@`& ";//33 (? = 2)
const std::u32string Numbers = U"0123456789";
const std::u32string EnglishAlph = U"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const std::u32string RussianAlph = U"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя";

const std::u32string AllCharacters = Symbols + Numbers + EnglishAlph + RussianAlph;

class Font{
public:
	Font(std::string);
	Font(std::string, Logger*);
	~Font();
	Texture *FontTexture;
	float FontRatio = 2.5f;
	bool IsLoaded();
	unsigned int AddStringToDraw(std::u32string, float, float, float);
	//void DeleteString(unsigned int);
	float GetStringWidth(std::u32string, float);
	void ClearBuffers();
	unsigned long int GenerateVAO(GLuint&, GLuint&, GLuint&);
private:
	Logger *FontLogger;
	bool Logging;
	int GetCharacterID(char32_t);
	unsigned int CurrentLine = 0;
	Vertex **VertexBuffers = NULL;
	unsigned long int *StringLengths = NULL;
	GLuint **IndexBuffers = NULL;
};

#endif // FONT_H
