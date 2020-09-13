#include "Font.h"
Font::Font(std::string textureFilePath){
	this->FontTexture = new Texture(textureFilePath, GL_TEXTURE_2D);
	this->Logging = false;
}

Font::Font(std::string textureFilePath, Logger *FontLogger){
	if (FontLogger != NULL){
		this->Logging = true;
		this->FontLogger = FontLogger;
		this->FontLogger->AddToLog("Loading font textures...");
		this->FontTexture = new Texture(textureFilePath, GL_TEXTURE_2D, FontLogger);
	} else {
		this->Logging = false;
		this->FontTexture = new Texture(textureFilePath, GL_TEXTURE_2D);
	}
}

Font::~Font(){
	if (this->Logging)
		this->FontLogger->AddToLog("Deleting font texture...");
	delete this->FontTexture;
	if (this->Logging)
		this->FontLogger->AddToLog("Clearing font buffers...");
	this->ClearBuffers();
}

int Font::GetCharacterID(char32_t character){
	long unsigned int i = 0;
	for (; (i < AllCharacters.length()) && (AllCharacters[i] != character); ++i);
	if (i == AllCharacters.length())
		return 2;
	else
		return i;
}

unsigned int Font::AddStringToDraw(std::u32string TextString, float x, float y, float TextHeight){
	Vertex **VertexBuffersBUF = new Vertex*[CurrentLine+1];
	if (this->VertexBuffers != NULL){
		for (unsigned int i = 0; i < CurrentLine; ++i){
			VertexBuffersBUF[i] = this->VertexBuffers[i];
		}
		delete []this->VertexBuffers;
	}
	this->VertexBuffers = VertexBuffersBUF;
	this->VertexBuffers[CurrentLine] = new Vertex[4*TextString.length()];
	
	GLuint **IndexBuffersBUF = new GLuint*[CurrentLine+1];
	if (this->IndexBuffers != NULL){
		for (unsigned int i = 0; i < CurrentLine; ++i){
			IndexBuffersBUF[i] = this->IndexBuffers[i];
		}
		delete []this->IndexBuffers;
	}
	this->IndexBuffers = IndexBuffersBUF;
	this->IndexBuffers[CurrentLine] = new GLuint[6*TextString.length()];
	
	float TextWidth = TextHeight/this->FontRatio;
	float Spacing = TextWidth/10;
	glm::vec3 right(TextWidth, 0.f, 0.f);
	glm::vec3 down(0.f, -TextHeight, 0.f);
	for (long unsigned int i = 0; i < TextString.length(); ++i){
		glm::vec3 baseLoc(x + i*(TextWidth + Spacing), y, 0.f);
		int ID = this->GetCharacterID(TextString[i]);
		int line = ID / SymbolsPerLine;
		int column = ID % SymbolsPerLine;
		glm::vec2 baseCha(1.f*column/SymbolsPerLine, 1.f*line/LineCount);
		Vertex *p = new Vertex(baseLoc, baseCha);
		VertexBuffers[CurrentLine][4*i+0] = *p;
		delete p;
		p = new Vertex(baseLoc + right, baseCha + glm::vec2(1.f/SymbolsPerLine, 0.f));
		VertexBuffers[CurrentLine][4*i+1] = *p;
		delete p;
		p = new Vertex(baseLoc + down, baseCha + glm::vec2(0.f, 1.f/LineCount));
		VertexBuffers[CurrentLine][4*i+2] = *p;
		delete p;
		p = new Vertex(baseLoc + right + down, baseCha + glm::vec2(1.f/SymbolsPerLine, 1.f/LineCount));
		VertexBuffers[CurrentLine][4*i+3] = *p;
		delete p;
		IndexBuffers[CurrentLine][6*i+0] = (4*i+2);//0 1
		IndexBuffers[CurrentLine][6*i+1] = (4*i+1);//2 3
		IndexBuffers[CurrentLine][6*i+2] = (4*i+0);
		IndexBuffers[CurrentLine][6*i+3] = (4*i+2);
		IndexBuffers[CurrentLine][6*i+4] = (4*i+3);
		IndexBuffers[CurrentLine][6*i+5] = (4*i+1);
	}
	unsigned long int *StringLengthsBUF = new unsigned long int[CurrentLine + 1];
	if (this->StringLengths != NULL){
		for (unsigned int i = 0; i < CurrentLine; ++i)
			StringLengthsBUF[i] = this->StringLengths[i];
		delete []this->StringLengths;
	}
	this->StringLengths = StringLengthsBUF;
	StringLengths[CurrentLine] = TextString.length();
	++CurrentLine;
	return CurrentLine - 1;
}

/*void Font::DeleteString(unsigned int ID){
	
}*/

void Font::ClearBuffers(){
	for (unsigned int i = 0; i < this->CurrentLine; ++i){
		delete[] this->VertexBuffers[i];
		delete[] this->IndexBuffers[i];
	}
	delete[] this->VertexBuffers;
	this->VertexBuffers = NULL;
	delete[] this->IndexBuffers;
	this->IndexBuffers = NULL;
	delete[] this->StringLengths;
	this->StringLengths = NULL;
	this->CurrentLine = 0;
}

unsigned long int Font::GenerateVAO(GLuint &VAO, GLuint &VBO, GLuint &EBO){
	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
	
	unsigned long int TotalVertexCount = 0, TotalIndexCount = 0;
	for (unsigned int i = 0; i < CurrentLine; ++i){
		TotalVertexCount += 4*this->StringLengths[i];
		TotalIndexCount += 6*this->StringLengths[i];
	}
	Vertex *vertices = new Vertex[TotalVertexCount];
	GLuint *indices = new GLuint[TotalIndexCount];
	GLuint prevlast = 0;
	unsigned long int CurrentVertex = 0, CurrentIndex = 0;
	for (unsigned int i = 0; i < CurrentLine; ++i){
		for (unsigned long int j = 0; j < 4*StringLengths[i]; ++j){
			vertices[CurrentVertex] = this->VertexBuffers[i][j];
			++CurrentVertex;
		}
		for (unsigned long int j = 0; j < 6*StringLengths[i]; ++j){
			indices[CurrentIndex] = this->IndexBuffers[i][j] + prevlast;
			++CurrentIndex;
		}
		prevlast = CurrentVertex;
	}
	
	if (VBO != 0)
		glDeleteBuffers(1, &VBO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*TotalVertexCount, vertices, GL_STATIC_DRAW);
	
    if (EBO != 0)
		glDeleteBuffers(1, &EBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*TotalIndexCount, indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);
	
	return TotalIndexCount;
}

bool Font::IsLoaded(){	
	return true;
}

float Font::GetStringWidth(std::u32string TextString, float TextHeight){
	float TextWidth = TextHeight/this->FontRatio;
	float Spacing = TextWidth/10;
	return TextString.length()*(TextWidth+Spacing);
}

/*Just in case
void Font::GenerateSingleVAO(std::u32string TextString, float TextHeight, GLuint &VAO){
	Vertex *vertices = new Vertex[4*TextString.length()];
	GLuint *indices = new GLuint[6*TextString.length()];
	float TextWidth = TextHeight/2.5;
	float Spacing = TextWidth/10;
	glm::vec3 right(TextWidth, 0.f, 0.f);
	glm::vec3 down(0.f, -TextHeight, 0.f);
	for (long unsigned int i = 0; i < TextString.length(); ++i){
		glm::vec3 baseLoc(0.f + i*(TextWidth + Spacing), 0.f, 0.f);
		int ID = this->GetCharacterID(TextString[i]);
		int line = ID / SymbolsPerLine;
		int column = ID % SymbolsPerLine;
		glm::vec2 baseCha(1.f*column/SymbolsPerLine, 1.f*line/LineCount);
		Vertex *p = new Vertex(baseLoc, baseCha);
		vertices[4*i+0] = *p;
		delete p;
		p = new Vertex(baseLoc + right, baseCha + glm::vec2(1.f/SymbolsPerLine, 0.f));
		vertices[4*i+1] = *p;
		delete p;
		p = new Vertex(baseLoc + down, baseCha + glm::vec2(0.f, 1.f/LineCount));
		vertices[4*i+2] = *p;
		delete p;
		p = new Vertex(baseLoc + right + down, baseCha + glm::vec2(1.f/SymbolsPerLine, 1.f/LineCount));
		vertices[4*i+3] = *p;
		delete p;
		indices[6*i+0] = (4*i+2);//0 1
		indices[6*i+1] = (4*i+1);//2 3
		indices[6*i+2] = (4*i+0);
		indices[6*i+3] = (4*i+2);
		indices[6*i+4] = (4*i+3);
		indices[6*i+5] = (4*i+1);
	}
	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
	
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*4*TextString.length(), vertices, GL_STATIC_DRAW);
	
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6*TextString.length(), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(1);
} 
*/	