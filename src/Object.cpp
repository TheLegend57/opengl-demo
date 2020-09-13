#include "Object.h"

//Material Object
MaterialObject::MaterialObject(){
	
}

MaterialObject::~MaterialObject(){
	ResetObject();
}

void MaterialObject::LoadObject(std::string ObjectFilePath){	
	std::vector<glm::vec3> VertexCoordinates(0, glm::vec3(0.f, 0.f, 0.f));
	std::ifstream ObjectFile;
	ObjectFile.open(ObjectFilePath, std::ifstream::in);
	std::string input;
	
	//Load materials file
	std::getline(ObjectFile, input);
	while (input.find("mtllib") == std::string::npos)
		std::getline(ObjectFile, input);
	while (input[0] == ' ')
		input.erase(0, 1);
	input.erase(0, 6);
	while (input[0] == ' ')
		input.erase(0, 1);
	std::ifstream MaterialFile;
	unsigned int SlashPos = ObjectFilePath.find_last_of("/");
	ObjectFilePath.erase(SlashPos, ObjectFilePath.length() - SlashPos);
	MaterialFile.open(ObjectFilePath + "/" + input);
	std::getline(MaterialFile, input);
	this->materials = new Material[1];
	++this->TotalSubParts;
	while(input.find("newmtl") == std::string::npos)
		std::getline(MaterialFile, input);
	while (input[0] == ' ')
		input.erase(0, 1);
	input.erase(0, 7);
	this->materials[0].MaterialName = input;
	std::getline(MaterialFile, input);
	while((input.find("newmtl") == std::string::npos) && (!MaterialFile.eof())){
		while (input[0] == ' ')
			input.erase(0, 1);
		if (input.find("Ns") != std::string::npos){
			input.erase(0, 3);
			this->materials[0].SpecularExponent = std::stof(input + "f");
		} else if (input.find("Ka") != std::string::npos){
			input.erase(0, 3);
			std::string ColorString = "";
			unsigned int i = 0;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				ColorString += input[i];
			ColorString += "f";
			float r = std::stof(ColorString);
			ColorString = "";
			++i;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				ColorString += input[i];
			ColorString += "f";
			float g = std::stof(ColorString);
			ColorString = "";
			++i;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				ColorString += input[i];
			ColorString += "f";
			float b = std::stof(ColorString);
			this->materials[0].AmbientColor = glm::vec3(r, g, b);
		} else if (input.find("Kd") != std::string::npos){
			input.erase(0, 3);
			std::string ColorString = "";
			unsigned int i = 0;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				ColorString += input[i];
			ColorString += "f";
			float r = std::stof(ColorString);
			ColorString = "";
			++i;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				ColorString += input[i];
			ColorString += "f";
			float g = std::stof(ColorString);
			ColorString = "";
			++i;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				ColorString += input[i];
			ColorString += "f";
			float b = std::stof(ColorString);
			this->materials[0].DiffuseColor = glm::vec3(r, g, b);
			
		} else if (input.find("Ks") != std::string::npos){
			input.erase(0, 3);
			std::string ColorString = "";
			unsigned int i = 0;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				ColorString += input[i];
			ColorString += "f";
			float r = std::stof(ColorString);
			ColorString = "";
			++i;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				ColorString += input[i];
			ColorString += "f";
			float g = std::stof(ColorString);
			ColorString = "";
			++i;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				ColorString += input[i];
			ColorString += "f";
			float b = std::stof(ColorString);
			this->materials[0].SpecularColor = glm::vec3(r, g, b);
		} else if (input.find("Ke") != std::string::npos){
			input.erase(0, 3);
			std::string ColorString = "";
			unsigned int i = 0;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				ColorString += input[i];
			ColorString += "f";
			float r = std::stof(ColorString);
			ColorString = "";
			++i;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				ColorString += input[i];
			ColorString += "f";
			float g = std::stof(ColorString);
			ColorString = "";
			++i;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				ColorString += input[i];
			ColorString += "f";
			float b = std::stof(ColorString);
			this->materials[0].EmissionColor = glm::vec3(r, g, b);
		} else if (input.find("Ni") != std::string::npos){
			input.erase(0, 3);
			this->materials[0].OpticalDensity = std::stof(input + "f");
		} else if (input.find("d") != std::string::npos){
			input.erase(0, 2);
			this->materials[0].Transparency = 1 - std::stof(input + "f");
		} else if (input.find("Tr") != std::string::npos){
			input.erase(0, 3);
			this->materials[0].Transparency = std::stof(input + "f");
		}
		if (!MaterialFile.eof())
			std::getline(MaterialFile, input);
	}
	while (!MaterialFile.eof()){
		Material *Buffer = new Material[this->TotalSubParts + 1];
		for (unsigned int i = 0; i < this->TotalSubParts; ++i)
			Buffer[i] = this->materials[i];
		delete[] this->materials;
		++this->TotalSubParts;
		this->materials = Buffer;
		while(input.find("newmtl") == std::string::npos)
			std::getline(MaterialFile, input);
		while (input[0] == ' ')
			input.erase(0, 1);
		input.erase(0, 7);
		this->materials[this->TotalSubParts-1].MaterialName = input;
		std::getline(MaterialFile, input);
		while((input.find("newmtl") == std::string::npos) && (!MaterialFile.eof())){
			while (input[0] == ' ')
				input.erase(0, 1);
			if (input.find("Ns") != std::string::npos){
				input.erase(0, 3);
				this->materials[this->TotalSubParts-1].SpecularExponent = std::stof(input + "f");
			} else if (input.find("Ka") != std::string::npos){
				input.erase(0, 3);
				std::string ColorString = "";
				unsigned int i = 0;
				for (; (input[i] != ' ') && (i < input.length()); ++i)
					ColorString += input[i];
				ColorString += "f";
				float r = std::stof(ColorString);
				ColorString = "";
				++i;
				for (; (input[i] != ' ') && (i < input.length()); ++i)
					ColorString += input[i];
				ColorString += "f";
				float g = std::stof(ColorString);
				ColorString = "";
				++i;
				for (; (input[i] != ' ') && (i < input.length()); ++i)
					ColorString += input[i];
				ColorString += "f";
				float b = std::stof(ColorString);
				this->materials[this->TotalSubParts-1].AmbientColor = glm::vec3(r, g, b);
			} else if (input.find("Kd") != std::string::npos){
				input.erase(0, 3);
				std::string ColorString = "";
				unsigned int i = 0;
				for (; (input[i] != ' ') && (i < input.length()); ++i)
					ColorString += input[i];
				ColorString += "f";
				float r = std::stof(ColorString);
				ColorString = "";
				++i;
				for (; (input[i] != ' ') && (i < input.length()); ++i)
					ColorString += input[i];
				ColorString += "f";
				float g = std::stof(ColorString);
				ColorString = "";
				++i;
				for (; (input[i] != ' ') && (i < input.length()); ++i)
					ColorString += input[i];
				ColorString += "f";
				float b = std::stof(ColorString);
				this->materials[this->TotalSubParts-1].DiffuseColor = glm::vec3(r, g, b);
			} else if (input.find("Ks") != std::string::npos){
				input.erase(0, 3);
				std::string ColorString = "";
				unsigned int i = 0;
				for (; (input[i] != ' ') && (i < input.length()); ++i)
					ColorString += input[i];
				ColorString += "f";
				float r = std::stof(ColorString);
				ColorString = "";
				++i;
				for (; (input[i] != ' ') && (i < input.length()); ++i)
					ColorString += input[i];
				ColorString += "f";
				float g = std::stof(ColorString);
				ColorString = "";
				++i;
				for (; (input[i] != ' ') && (i < input.length()); ++i)
					ColorString += input[i];
				ColorString += "f";
				float b = std::stof(ColorString);
				this->materials[this->TotalSubParts-1].SpecularColor = glm::vec3(r, g, b);
			} else if (input.find("Ke") != std::string::npos){
				input.erase(0, 3);
				std::string ColorString = "";
				unsigned int i = 0;
				for (; (input[i] != ' ') && (i < input.length()); ++i)
					ColorString += input[i];
				ColorString += "f";
				float r = std::stof(ColorString);
				ColorString = "";
				++i;
				for (; (input[i] != ' ') && (i < input.length()); ++i)
					ColorString += input[i];
				ColorString += "f";
				float g = std::stof(ColorString);
				ColorString = "";
				++i;
				for (; (input[i] != ' ') && (i < input.length()); ++i)
					ColorString += input[i];
				ColorString += "f";
				float b = std::stof(ColorString);
				this->materials[this->TotalSubParts-1].EmissionColor = glm::vec3(r, g, b);
			} else if (input.find("Ni") != std::string::npos){
				input.erase(0, 3);
				this->materials[this->TotalSubParts-1].OpticalDensity = std::stof(input + "f");
			} else if (input.find("d") != std::string::npos){
				input.erase(0, 2);
				this->materials[this->TotalSubParts-1].Transparency = 1 - std::stof(input + "f");
			} else if (input.find("Tr") != std::string::npos){
				input.erase(0, 3);
				this->materials[this->TotalSubParts-1].Transparency = std::stof(input + "f");
			}
			std::getline(MaterialFile, input);
		}
	}
	//Finish loading materials file
	
	//Continue with main object file
	std::getline(ObjectFile, input);
	while (input[0] == ' ')
		input.erase(0, 1);
	while (input[0] != 'o'){
		std::getline(ObjectFile, input);
		while (input[0] == ' ')
			input.erase(0, 1);
	}
	std::getline(ObjectFile, input);
	while (input[0] == ' ')
		input.erase(0, 1);
	//Load vertices
	while ((input[0] == 'v') || (input[0] == '#')){
		if (input[0] != '#'){
			input.erase(0, 2);
			std::string CoordinateString = "";
			unsigned int i = 0;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				CoordinateString += input[i];
			CoordinateString += "f";
			float x = std::stof(CoordinateString);
			CoordinateString = "";
			++i;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				CoordinateString += input[i];
			CoordinateString += "f";
			float y = std::stof(CoordinateString);
			CoordinateString = "";
			++i;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				CoordinateString += input[i];
			CoordinateString += "f";
			float z = std::stof(CoordinateString);
			VertexCoordinates.push_back(glm::vec3(x, y, z));
		}
		std::getline(ObjectFile, input);
		while (input[0] == ' ')
			input.erase(0, 1);
	}
	//Finished loading vertices
	this->meshes = new MaterialMesh[this->TotalSubParts];
	unsigned int CurrentMesh = 0;
	while (!ObjectFile.eof() && (CurrentMesh < this->TotalSubParts)){
		while (input.find("usemtl") == std::string::npos)
			std::getline(ObjectFile, input);
		while (input[0] == ' ')
			input.erase(0, 1);
		input.erase(0, 7);
		if (input != materials[CurrentMesh].MaterialName){
			bool found = false;
			for (unsigned int i = 0; (i < this->TotalSubParts) && !found; ++i)
				if (materials[i].MaterialName == input){
					found = true;
					Material BUF;
					BUF = this->materials[i];
					this->materials[i] = this->materials[CurrentMesh];
					this->materials[CurrentMesh] = BUF;
 				}
		}
		std::getline(ObjectFile, input);
		if (input == "s off")
			std::getline(ObjectFile, input);
		while (input[0] == ' ')
			input.erase(0, 1);
		while ((input[0] == 'f') || (input[0] == '#')){
			input.erase(0, 2);
			std::string IndexString = "";
			unsigned int i = 0;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				IndexString += input[i];
			unsigned int id1 = std::stoi(IndexString) - 1;
			IndexString = "";
			++i;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				IndexString += input[i];
			unsigned int id2 = std::stoi(IndexString) - 1;
			IndexString = "";
			++i;
			for (; (input[i] != ' ') && (i < input.length()); ++i)
				IndexString += input[i];
			unsigned int id3 = std::stoi(IndexString) - 1;
			this->meshes[CurrentMesh].AddVertex(VertexCoordinates[id1]);
			this->meshes[CurrentMesh].AddVertex(VertexCoordinates[id2]);
			this->meshes[CurrentMesh].AddVertex(VertexCoordinates[id3]);			
			std::getline(ObjectFile, input);
			while (input[0] == ' ')
				input.erase(0, 1);
		}
		++CurrentMesh;
	}
	this->ObjectLoaded = true;
}

void MaterialObject::SetUpShader(Shader *MaterialShader){
	if (MaterialShader != NULL){
		this->MaterialShader = MaterialShader;
		this->ShaderSetUp = true;
	}
}

void MaterialObject::DrawObject(glm::mat4 ModelMatrix, Camera *ObjectCamera, DirectionalLight Light){
	if (!VAOsGenerated)
		this->GenerateVAOs();
	if (this->ObjectLoaded && this->ShaderSetUp){
		for (unsigned int i = 0; i < this->TotalSubParts; ++i){
			//Set up shader parameters
			this->MaterialShader->SetUniformMat4("ModelMatrix", ModelMatrix, GL_FALSE);
			this->MaterialShader->SetUniformMat4("ViewMatrix", ObjectCamera->getViewMatrix(), GL_FALSE);
			this->MaterialShader->SetUniformMat4("ProjectionMatrix", ObjectCamera->getProjectionMatrix(), GL_FALSE);
			this->MaterialShader->SetUniform3f("CameraPosition", ObjectCamera->getPosition());			
			
			this->MaterialShader->SetUniform1f("material.SpecularExponent", this->materials[i].SpecularExponent);
			this->MaterialShader->SetUniform3f("material.AmbientColor", this->materials[i].AmbientColor);
			this->MaterialShader->SetUniform3f("material.DiffuseColor", this->materials[i].DiffuseColor);
			this->MaterialShader->SetUniform3f("material.SpecularColor", this->materials[i].SpecularColor);
			this->MaterialShader->SetUniform3f("material.EmissionColor", this->materials[i].EmissionColor);
			this->MaterialShader->SetUniform1f("material.OpticalDensity", this->materials[i].OpticalDensity);
			this->MaterialShader->SetUniform1f("material.Transparency", this->materials[i].Transparency);
			
			this->MaterialShader->SetUniform1f("light.Strength", Light.Strength);
			this->MaterialShader->SetUniform3f("light.Direction", Light.Direction);
			this->MaterialShader->SetUniform3f("light.LightColor", Light.LightColor);
			//
			glBindVertexArray(this->VAOs[i]);
			glDrawArrays(GL_TRIANGLES, 0, this->VertexCounts[i]);
		}
	}
}

void MaterialObject::ResetObject(){
	delete[] this->materials;
	this->materials = NULL;
	delete[] this->meshes;
	this->meshes = NULL;
	delete[] this->VAOs;
	this->VAOs = NULL;
	delete[] this->VertexCounts;
	this->VertexCounts = NULL;
	this->TotalSubParts = 0;
	this->ShaderSetUp = false;
	this->ObjectLoaded = false;
	this->VAOsGenerated = false;
}

void MaterialObject::GenerateVAOs(){
	if (!this->VAOsGenerated){
		this->VAOs = new GLuint[this->TotalSubParts];
		this->VertexCounts = new unsigned int[this->TotalSubParts];
		for (unsigned int i = 0; i < this->TotalSubParts; ++i)
			this->VertexCounts[i] = this->meshes[i].GenerateVAO(VAOs[i]);
		this->VAOsGenerated = true;
	}
}

//Other types...
