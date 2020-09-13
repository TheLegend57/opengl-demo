#include "Button.h"

Button::Button(float x, float y, float w, float h, float uvx, float uvy, float uvw, float uvh, std::u32string ButtonTextString){
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
	this->uvx = uvx;
	this->uvy = uvy;
	this->uvwidth = uvw;
	this->uvheight = uvh;
	this->ButtonTextString = ButtonTextString;
}

Button::Button(){
	this->x = 0.f;
	this->y = 0.f;
	this->width = 0.f;
	this->height = 0.f;
	this->uvx = 0.f;
	this->uvy = 0.f;
	this->uvwidth = 0.f;
	this->uvheight = 0.f;
	this->ButtonTextString = U"";
}

Button::~Button(){
}

bool Button::IsMouseInside(float mouseX, float mouseY){
	if (mouseX <= this->x + this->width)
		if (mouseX >= this->x)
			if (mouseY >= this->y - this->height)
				if (mouseY <= this->y)
					return true;
	return false;
}
