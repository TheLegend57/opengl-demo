#ifndef TEXTBOX_H
#define TEXTBOX_H
#include "Texture.h"
#include <string>
class TextBox{
public:
	TextBox();
	~TextBox();
	void SetString(std::u32string);
	std::u32string GetString();
	bool IsMouseInside(float, float);
	void ChangeStatus(unsigned short int);
	
	void MoveCursorLeft();
	void MoveCursorRight();
	void SetCursorPos(unsigned long int);
	
	void AddCharacter(char32_t);
	void RemoveCharacterLeft();
	void RemoveCharacterRight();
		
private:
	std::u32string TextString;
	float x, y, width, height, uv_x, uv_y, uv_width, uv_height; 
	unsigned long int CursorPos;
	unsigned short int status = 0;
	Texture *MenuTextures;
};
#endif /* TEXTBOX_H */