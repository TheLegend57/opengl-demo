#include "Menu.h"
Menu::Menu(std::string TexturePath){
	this->MenuTexture = new Texture(TexturePath, GL_TEXTURE_2D);
}

Menu::~Menu(){
	delete MenuTexture;
	this->ResetMenu();
}

unsigned int Menu::AddButton(Button *AddedButton){
	Button **ButtonListBUF = new Button*[CurrentButton + 1];
	for (unsigned int i = 0; i < CurrentButton; ++i)
		ButtonListBUF[i] = ButtonList[i];
	if (ButtonList != NULL)
		delete[] ButtonList;
	ButtonList = ButtonListBUF;
	ButtonList[CurrentButton] = AddedButton;
	++CurrentButton;
	return CurrentButton - 1;
}

bool Menu::CheckButtons(float x, float y, unsigned int &ButtonID){
	unsigned int i = 0;
	for (; i < CurrentButton; ++i)
		if (ButtonList[i]->IsMouseInside(x, y))
			break;
	ButtonID = i;	
	return (i < CurrentButton);
}

void Menu::ChangeButtonState(unsigned int ButtonID, unsigned short int NewState){
	if (ButtonID < CurrentButton){
		if ((NewState == 1) || (NewState == 2)){
			if (this->WasButtonHoveredOrPressed)
				ButtonList[this->LastHoveredOrPressed]->state = 0;
			this->LastHoveredOrPressed = ButtonID;
			this->WasButtonHoveredOrPressed = true;
		}
		ButtonList[ButtonID]->state = NewState;
	}
}

void Menu::ResetPressedOrHoveredButton(){
	if (this->WasButtonHoveredOrPressed){
		ButtonList[this->LastHoveredOrPressed]->state = 0;
		this->LastHoveredOrPressed = 0;
		this->WasButtonHoveredOrPressed = false;
	}
}

void Menu::AddImage(MenuImage AddedImage){
	MenuImage *ImageListBUF = new MenuImage[CurrentImage + 1];
	for (unsigned int i = 0; i < CurrentImage; ++i)
		ImageListBUF[i] = ImageList[i];
	if (ImageList != NULL)
		delete[] ImageList;
	ImageList = ImageListBUF;
	ImageList[CurrentImage] = AddedImage;
	++CurrentImage;
}

void Menu::ResetMenu(){
	if (ButtonList != NULL){
		for (unsigned int i = 0; i < CurrentButton; ++i)
			delete ButtonList[i];
		delete[] ButtonList;
	}
	if (ImageList != NULL)
		delete[] ImageList;
	ButtonList = NULL;
	ImageList = NULL;
	this->CurrentButton = 0;
	this->CurrentImage = 0;
	this->LastHoveredOrPressed = 0;
	this->WasButtonHoveredOrPressed = false;
	glDeleteBuffers(1, &this->MenuVBO);
	this->MenuVBO = 0;
	glDeleteBuffers(1, &this->MenuEBO);
	this->MenuEBO = 0;
	glDeleteBuffers(1, &this->FontVBO);
	this->FontVBO = 0;
	glDeleteBuffers(1, &this->FontEBO);
	this->FontEBO = 0;
	glDeleteVertexArrays(1, &this->MenuVAO);
	this->MenuVAO = 0;
	glDeleteVertexArrays(1, &this->FontVAO);
	this->FontVAO = 0;
}

unsigned long int Menu::GenerateVAOs(GLuint &VAO, Font *ButtonFont, GLuint &FontVAOp, unsigned long int &FontIndexCount){	
	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
	this->MenuVAO = VAO;
	
	unsigned long int TotalVertexCount = 0, TotalIndexCount = 0;
	for (unsigned int i = 0; i < CurrentImage; ++i){
		TotalVertexCount += 4;
		TotalIndexCount += 6;
	}
	for (unsigned int i = 0; i < CurrentButton; ++i){
		TotalVertexCount += 4;
		TotalIndexCount += 6;
	}
	Vertex *vertices = new Vertex[TotalVertexCount];
	GLuint *indices = new GLuint[TotalIndexCount];
	unsigned int i = 0;
	for (; i < CurrentImage; ++i){
		glm::vec3 baseLoc(ImageList[i].x, ImageList[i].y, 0.f);
		glm::vec3 right(ImageList[i].width, 0.f, 0.f);
		glm::vec3 down(0.f, 0.f - ImageList[i].height, 0.f);
		glm::vec2 baseUV(ImageList[i].uvx, ImageList[i].uvy);
		Vertex *p = new Vertex(baseLoc, baseUV);
		vertices[4*i+0] = *p;
		delete p;
		p = new Vertex(baseLoc + right, baseUV + glm::vec2(ImageList[i].uvwidth, 0.f));
		vertices[4*i+1] = *p;
		delete p;
		p = new Vertex(baseLoc + down, baseUV + glm::vec2(0.f, ImageList[i].uvheight));
		vertices[4*i+2] = *p;
		delete p;
		p = new Vertex(baseLoc + right + down, baseUV + glm::vec2(ImageList[i].uvwidth, ImageList[i].uvheight));
		vertices[4*i+3] = *p;
		delete p;
		indices[6*i+0] = (4*i+2);//0 1
		indices[6*i+1] = (4*i+1);//2 3
		indices[6*i+2] = (4*i+0);
		indices[6*i+3] = (4*i+2);
		indices[6*i+4] = (4*i+3);
		indices[6*i+5] = (4*i+1);
	}
	unsigned int delta = i;
	for (; i - delta < CurrentButton; ++i){
		glm::vec3 baseLoc(ButtonList[i-delta]->x, ButtonList[i-delta]->y, 0.0f);
		glm::vec3 right(ButtonList[i-delta]->width, 0.f, 0.f);
		glm::vec3 down(0.f, 0.f - ButtonList[i-delta]->height, 0.f);
		glm::vec2 baseUV(ButtonList[i-delta]->uvx, ButtonList[i-delta]->uvy + ButtonList[i-delta]->state * ButtonList[i-delta]->uvheight);
		Vertex *p = new Vertex(baseLoc, baseUV);
		vertices[4*i+0] = *p;
		delete p;
		p = new Vertex(baseLoc + right, baseUV + glm::vec2(ButtonList[i-delta]->uvwidth, 0.f));
		vertices[4*i+1] = *p;
		delete p;
		p = new Vertex(baseLoc + down, baseUV + glm::vec2(0.f, ButtonList[i-delta]->uvheight));
		vertices[4*i+2] = *p;
		delete p;
		p = new Vertex(baseLoc + right + down, baseUV + glm::vec2(ButtonList[i-delta]->uvwidth, ButtonList[i-delta]->uvheight));
		vertices[4*i+3] = *p;
		delete p;
		indices[6*i+0] = (4*i+2);//0 1
		indices[6*i+1] = (4*i+1);//2 3
		indices[6*i+2] = (4*i+0);
		indices[6*i+3] = (4*i+2);
		indices[6*i+4] = (4*i+3);
		indices[6*i+5] = (4*i+1);
	}
	
	if (this->MenuVBO != 0)
		glDeleteBuffers(1, &this->MenuVBO);
    glGenBuffers(1, &this->MenuVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->MenuVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*TotalVertexCount, vertices, GL_STATIC_DRAW);
	
    if (this->MenuEBO != 0)
		glDeleteBuffers(1, &this->MenuEBO);
    glGenBuffers(1, &this->MenuEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->MenuEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*TotalIndexCount, indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	i = 0;
	for (; i < CurrentButton; ++i){
		ButtonFont->AddStringToDraw(ButtonList[i]->ButtonTextString, ButtonList[i]->x + ButtonList[i]->width/20, ButtonList[i]->y - ButtonList[i]->height/20, ButtonList[i]->height*0.9f);
	}
	FontIndexCount = ButtonFont->GenerateVAO(FontVAOp, this->FontVBO, this->FontEBO);
	this->FontVAO = FontVAOp;
	ButtonFont->ClearBuffers();
	return TotalIndexCount;
}

unsigned long int Menu::UpdateButtonsAndImages(GLuint &VAO){
	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
	this->MenuVAO = VAO;
	
	unsigned long int TotalVertexCount = 0, TotalIndexCount = 0;
	for (unsigned int i = 0; i < CurrentImage; ++i){
		TotalVertexCount += 4;
		TotalIndexCount += 6;
	}
	for (unsigned int i = 0; i < CurrentButton; ++i){
		TotalVertexCount += 4;
		TotalIndexCount += 6;
	}
	Vertex *vertices = new Vertex[TotalVertexCount];
	GLuint *indices = new GLuint[TotalIndexCount];
	unsigned int i = 0;
	for (; i < CurrentImage; ++i){
		glm::vec3 baseLoc(ImageList[i].x, ImageList[i].y, 0.f);
		glm::vec3 right(ImageList[i].width, 0.f, 0.f);
		glm::vec3 down(0.f, 0.f - ImageList[i].height, 0.f);
		glm::vec2 baseUV(ImageList[i].uvx, ImageList[i].uvy);
		Vertex *p = new Vertex(baseLoc, baseUV);
		vertices[4*i+0] = *p;
		delete p;
		p = new Vertex(baseLoc + right, baseUV + glm::vec2(ImageList[i].uvwidth, 0.f));
		vertices[4*i+1] = *p;
		delete p;
		p = new Vertex(baseLoc + down, baseUV + glm::vec2(0.f, ImageList[i].uvheight));
		vertices[4*i+2] = *p;
		delete p;
		p = new Vertex(baseLoc + right + down, baseUV + glm::vec2(ImageList[i].uvwidth, ImageList[i].uvheight));
		vertices[4*i+3] = *p;
		delete p;
		indices[6*i+0] = (4*i+2);//0 1
		indices[6*i+1] = (4*i+1);//2 3
		indices[6*i+2] = (4*i+0);
		indices[6*i+3] = (4*i+2);
		indices[6*i+4] = (4*i+3);
		indices[6*i+5] = (4*i+1);
	}
	unsigned int delta = i;
	for (; i - delta < CurrentButton; ++i){
		glm::vec3 baseLoc(ButtonList[i-delta]->x, ButtonList[i-delta]->y, 0.0f);
		glm::vec3 right(ButtonList[i-delta]->width, 0.f, 0.f);
		glm::vec3 down(0.f, 0.f - ButtonList[i-delta]->height, 0.f);
		glm::vec2 baseUV(ButtonList[i-delta]->uvx, ButtonList[i-delta]->uvy + ButtonList[i-delta]->state * ButtonList[i-delta]->uvheight);
		Vertex *p = new Vertex(baseLoc, baseUV);
		vertices[4*i+0] = *p;
		delete p;
		p = new Vertex(baseLoc + right, baseUV + glm::vec2(ButtonList[i-delta]->uvwidth, 0.f));
		vertices[4*i+1] = *p;
		delete p;
		p = new Vertex(baseLoc + down, baseUV + glm::vec2(0.f, ButtonList[i-delta]->uvheight));
		vertices[4*i+2] = *p;
		delete p;
		p = new Vertex(baseLoc + right + down, baseUV + glm::vec2(ButtonList[i-delta]->uvwidth, ButtonList[i-delta]->uvheight));
		vertices[4*i+3] = *p;
		delete p;
		indices[6*i+0] = (4*i+2);//0 1
		indices[6*i+1] = (4*i+1);//2 3
		indices[6*i+2] = (4*i+0);
		indices[6*i+3] = (4*i+2);
		indices[6*i+4] = (4*i+3);
		indices[6*i+5] = (4*i+1);
	}
	
	if (this->MenuVBO != 0)
		glDeleteBuffers(1, &this->MenuVBO);
    glGenBuffers(1, &this->MenuVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->MenuVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*TotalVertexCount, vertices, GL_STATIC_DRAW);
	
    if (this->MenuEBO != 0)
		glDeleteBuffers(1, &this->MenuEBO);
    glGenBuffers(1, &this->MenuEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->MenuEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*TotalIndexCount, indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	
	return TotalIndexCount;
}

unsigned short int Menu::GetButtonState(unsigned int ButtonID){
	if (ButtonID < CurrentButton)
		return ButtonList[ButtonID]->state;
	else
		return 0;
}