#ifndef MENU_H
#define MENU_H
#include "Button.h"
#include "Font.h"

struct MenuImage{
	float x, y, width, height, uvx, uvy, uvwidth, uvheight;
};

class Menu{
public:
	Menu(std::string);
	~Menu();
	unsigned int AddButton(Button*);
	bool CheckButtons(float, float, unsigned int&);
	void AddImage(MenuImage);//x, y, w, h, ux, uy, uw, uh 
	unsigned long int GenerateVAOs(GLuint&, Font*, GLuint&, unsigned long int&);
	unsigned long int UpdateButtonsAndImages(GLuint&);
	void ChangeButtonState(unsigned int, unsigned short int);
	unsigned short int GetButtonState(unsigned int);
	void ResetPressedOrHoveredButton();
	void ResetMenu();
	Texture *MenuTexture;
private:
	unsigned int LastHoveredOrPressed = 0;
	bool WasButtonHoveredOrPressed = false;
	Button **ButtonList = NULL;
	unsigned int CurrentButton = 0;
	MenuImage *ImageList = NULL;
	unsigned int CurrentImage = 0;
	GLuint MenuVAO = 0, MenuVBO = 0, MenuEBO = 0;
	GLuint FontVAO = 0, FontVBO = 0, FontEBO = 0;
};
#endif // MENU_H