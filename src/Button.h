#ifndef BUTTON_H
#define BUTTON_H
#include <string>

class Button{
public:
	Button(float, float, float, float, float, float, float, float, std::u32string);
	Button();
	~Button();
	bool IsMouseInside(float, float);
	unsigned short int state = 0;//0 - default, 1 - hovered, 2 - pressed, 3 - inactive/locked
	float x, y, width, height;
	float uvx, uvy, uvwidth, uvheight;
	std::u32string ButtonTextString;	
};

#endif // BUTTON_H
